#include "homeworks/tema_2/MainEvent.h"

using namespace std;
using namespace tema2;

MainEvent::MainEvent()
{
    // parametri pentru matricea de proiectie
    zFar = 100.f;
    zNear = 0.1f;
    fov = 45;
    aspectRatio = window->props.aspectRatio;
    // Initializare drum
    glm::vec3 road_scale = glm::vec3(1, 1, 1);
    road = new Road(0.1, road_scale , road_color);

    // Parametri ortografici pentru camera mini map
    left = -road_scale.x;
    right = road_scale.x;
    bottom = left;
    top = right;

    // Initializare teren
    terrain = new Terrain(road->min_x, road->max_x, road->min_z, road->max_z, road->height - 0.01, terrain_color);

    float scale_factor = 2 * road->band_width / 4;
    car_max_length = scale_factor;
    car_max_height = scale_factor * 0.5f;
    car_max_width = scale_factor * 0.4f;
    number_of_bots = NUMBER_OF_BOTS;

    glm::vec3 player_color = NormalizedRGB(random_in_a_range(0, 255), random_in_a_range(0, 255), random_in_a_range(0, 255));
    player = new Player("player", glm::vec3(car_max_length, car_max_height, car_max_width), player_color, road);

    curvature_scale_factor = -player->scale.x / (10.f * road_scale.x);

    show_road_outline = false;
}


MainEvent::~MainEvent()
{
}


void MainEvent::Init()
{
    // Initializare boti
    for(int i = 0; i < number_of_bots; i++)
    {
        string name = "bot_" + std::to_string(i);
        glm::vec3 bot_color = NormalizedRGB(random_in_a_range(0, 255), random_in_a_range(0, 255), random_in_a_range(0, 255));
        float bot_length = random_in_a_range(0.8f * car_max_length, 1.1f * car_max_length);
        float bot_height = random_in_a_range(0.9f * car_max_height, 1.1f * car_max_height);
        float bot_width = random_in_a_range(0.8f * car_max_width, 1.1f * car_max_width);
        glm::vec3 bot_scale = { bot_length, bot_height, bot_width};
        Bot* new_bot = new Bot(name, bot_scale, bot_color);
        AddMeshToList(new_bot->bot_mesh);
        set_random_position_and_direction(new_bot);
        other_bots.push_back(new_bot);
    }

    // Initializare meshuri drum
    middleMeshCurve = road->create_middle_curve("middle");
    innerMeshCurve = road->create_inner_curve("inner");
    outerMeshCurve = road->create_outer_curve("outer");
    inner_lane_mesh = road->create_inner_lane_curve("inner_traffic");
    outer_lane_mesh = road->create_outter_lane_curve("outer_traffic");
    AddMeshToList(middleMeshCurve);
    AddMeshToList(innerMeshCurve);
    AddMeshToList(outerMeshCurve);
    AddMeshToList(inner_lane_mesh);
    AddMeshToList(outer_lane_mesh);
    roadMesh = road->road_mesh;

    // Initializare meshuri teren
    AddMeshToList(terrain->terrain_mesh);

    // Initializare jucator
    player_car = player->player_mesh;
    set_random_position_and_direction(player);
    AddMeshToList(player_car);

    // Initializeaza camere
    main_camera = new Camera(glm::vec3(0, 0, 0), glm::vec3(1, 0, -1), glm::vec3(0, 1, 0));
    mini_map_camera = new Camera(glm::vec3(0, 0, 0), glm::vec3(1, 0, -1), glm::vec3(0, 1, 0));

    // Copacii
    spawn_trees();

    // Matricile de proiectie pentru ambele camere
    main_camera->projectionMatrix = glm::perspective(RADIANS(fov), aspectRatio, zNear, zFar);
    mini_map_camera->projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);

	// Bind camera catre player
    bind_camera_to_player = true;

    // Initializare shader pentru efectul de curbura
    shader = new Shader("CurvatureShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::HOMEWORKS, "tema_2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::HOMEWORKS, "tema_2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;
}

void MainEvent::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(sky_color.r, sky_color.g, sky_color.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void MainEvent::Update(float deltaTimeSeconds)
{
    if (first_frame) first_frame = 0;
    else
    {
        do_calculations(deltaTimeSeconds);
        render_main_viewport();
        render_mini_map_viewport();
    }
}

void MainEvent::FrameEnd()
{
    main_camera->projectionMatrix = glm::perspective(RADIANS(fov), aspectRatio, zNear, zFar);
    mini_map_camera->projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
}

void MainEvent::render_mini_map_viewport()
{
    glClearColor(sky_color.r, sky_color.g, sky_color.b, 1);
    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    float new_x = resolution.x / 5;
    float new_y = new_x * 1.0 / aspectRatio;

    glViewport(20, resolution.y - new_y - 20, new_x, new_y);
    mini_map_camera->Set(player->position + glm::vec3(0, 10, 0), player->position, glm::vec3(0, 0, -1));
    render_scene(mini_map_camera);
}


void MainEvent::render_main_viewport()
{
    if (bind_camera_to_player)
    {
        glm::vec3 camera_pos = player->position - 4 * player->scale.x * player->forward;
        camera_pos += glm::vec3(0, player->scale.y * 2.5f, 0);
        glm::vec3 camera_look_at = player->position + glm::vec3(0, player->scale.y, 0) + player->scale.x * player->forward;
        main_camera->Set(camera_pos, camera_look_at, glm::vec3(0, 1, 0));
    }
    render_scene(main_camera);
}


void MainEvent::do_calculations(float deltaTimeSeconds)
{
    vector<glm::vec3> player_corners = player->get_player_car_corners();
    for (int i = 0; i < other_bots.size(); i++)
    {
        Bot* bot = other_bots[i];
        vector<glm::vec3> bot_corners = bot->get_bot_car_corners();
        for (glm::vec3 corner : player_corners)
        {
            bool ret = point_in_triangle(corner, bot_corners[0], bot_corners[1], bot_corners[2]);
            if (ret)
            {
                player->stop();
                break;
            }
            ret = point_in_triangle(corner, bot_corners[0], bot_corners[2], bot_corners[3]);
            if (ret)
            {
                player->stop();
                break;
            }
        }
        for (glm::vec3 corner : bot_corners)
        {
            bool ret = point_in_triangle(corner, player_corners[0], player_corners[1], player_corners[2]);
            if (ret)
            {
                player->stop();
                break;
            }
            ret = point_in_triangle(corner, player_corners[0], player_corners[2], player_corners[3]);
            if (ret)
            {
                player->stop();
                break;
            }
        }
        check_for_bot_next_waypoint(bot);
        bot->accelerate(deltaTimeSeconds);
        bot->move(deltaTimeSeconds);
    }

    player->move(deltaTimeSeconds);
}


void MainEvent::render_scene(Camera* camera)
{
    for(Bot* bot : other_bots)
    {
        RenderSimpleMesh(bot->bot_mesh, shader, bot->model_matrix, camera);
    }

    for (Tree* tree : trees)
    {
        RenderSimpleMesh(tree->tree_crown_mesh, shader, tree->tree_crown_model_matrix, camera);
        RenderSimpleMesh(tree->tree_trunk_mesh, shader, tree->tree_trunk_model_matrix, camera);
    }

    {
    	glm::mat4 modelMatrix = glm::mat4(1);
    	RenderSimpleMesh(terrain->terrain_mesh, shader, modelMatrix, camera);
    }

    if(show_road_outline)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderSimpleMesh(innerMeshCurve, shader, modelMatrix, camera);
        RenderSimpleMesh(middleMeshCurve, shader, modelMatrix, camera);
        RenderSimpleMesh(outerMeshCurve, shader, modelMatrix, camera);
        RenderSimpleMesh(inner_lane_mesh, shader, modelMatrix, camera);
        RenderSimpleMesh(outer_lane_mesh, shader, modelMatrix, camera);
    } else
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderSimpleMesh(roadMesh, shader, modelMatrix, camera);
    }

    RenderSimpleMesh(player_car, shader, player->model_matrix, camera);
}


void MainEvent::spawn_trees()
{
    float scale = road->band_width / 2;

    bool added_one = false;
    glm::vec3 last_added;
    int added = 0;
	for(int i = 0; i < road->outer_tree_lane.size(); i++)
	{
        glm::vec3 p1 = road->outer_tree_lane[i].position;
        if (added_one == true)
        {
            float distance = glm::length(last_added - p1);
            if(distance < 3 * scale) continue;
        }
        p1.y = terrain->height;
        float random_trunk_heigh = random_in_a_range(1.5f, 2.5f);
        glm::vec3 trunk_scale = { scale / 2, random_trunk_heigh * scale, scale / 2 };
        float random_crown_width = random_in_a_range(1.5f, 2.5f);
        glm::vec3 crown_scale = { random_crown_width * scale, random_crown_width * scale, random_crown_width * scale };
        glm::vec3 trunk_color = trunk_colors[rand() % trunk_colors.size()];
        glm::vec3 crown_color = crown_colors[rand() % crown_colors.size()];
        Tree* tree = new Tree("tree_" + std::to_string(added++), trunk_scale, crown_scale, p1, trunk_color, crown_color);
        AddMeshToList(tree->tree_crown_mesh);
        AddMeshToList(tree->tree_trunk_mesh);
        trees.push_back(tree);
        added_one = true;
        last_added = p1;
	}
    added_one = false;
    last_added = {};
    for (int i = 0; i < road->inner_tree_lane.size(); i++)
    {
        glm::vec3 p1 = road->inner_tree_lane[i].position;
        if (added_one == true)
        {
            float distance = glm::length(last_added - p1);
            if (distance < 3 * scale) continue;
        }
        float random_trunk_heigh = random_in_a_range(1.5f, 2.5f);
        glm::vec3 trunk_scale = { scale / 2, random_trunk_heigh * scale, scale / 2 };
        float random_crown_width = random_in_a_range(1.5f, 2.5f);
        glm::vec3 crown_scale = { random_crown_width * scale, random_crown_width * scale, random_crown_width * scale };
        glm::vec3 trunk_color = trunk_colors[rand() % trunk_colors.size()];
        glm::vec3 crown_color = crown_colors[rand() % crown_colors.size()];
        p1.y = terrain->height;
        Tree* tree = new Tree("tree_" + std::to_string(added++), trunk_scale, crown_scale, p1, trunk_color, crown_color);
        AddMeshToList(tree->tree_crown_mesh);
        AddMeshToList(tree->tree_trunk_mesh);
        trees.push_back(tree);
        added_one = true;
        last_added = p1;
    }

    for (int i = 0; i < OUT_OF_ROAD_TREES; i++)
    {
        glm::vec3 p1;
        do
        {
            p1 = { random_in_a_range(terrain->min_x, terrain->max_x), road->height, random_in_a_range(terrain->min_z, terrain->max_z) };
        } while (road->check_if_on_road(p1));
        p1.y = terrain->height;
        float random_trunk_heigh = random_in_a_range(1.5f, 2.5f);
        glm::vec3 trunk_scale = { scale / 2, random_trunk_heigh * scale, scale / 2 };
        float random_crown_width = random_in_a_range(1.5f, 2.5f);
        glm::vec3 crown_scale = { random_crown_width * scale, random_crown_width * scale, random_crown_width * scale };
        glm::vec3 trunk_color = trunk_colors[rand() % trunk_colors.size()];
        glm::vec3 crown_color = crown_colors[rand() % crown_colors.size()];
        Tree* tree = new Tree("tree_" + std::to_string(added++), trunk_scale, crown_scale, p1, trunk_color, crown_color);
        AddMeshToList(tree->tree_crown_mesh);
        AddMeshToList(tree->tree_trunk_mesh);
        trees.push_back(tree);
    }
}

void MainEvent::set_random_position_and_direction(Player* player)
{
    glm::vec3 spawn_point;
    glm::vec3 forward;
    if (rand() % 2 == 1)
    {
        int random_index = rand() % road->outer_traffic_lane.size();
        int next_index = get_next_or_prev_index_from_vector(random_index, road->outer_traffic_lane);
        spawn_point = road->outer_traffic_lane[random_index].position;
        glm::vec3 other_end = road->outer_traffic_lane[next_index].position;
        if (glm::length(other_end - spawn_point) == 0)
        {
            next_index = get_next_or_prev_index_from_vector(next_index, road->outer_traffic_lane);
            other_end = road->outer_traffic_lane[next_index].position;
        }
        forward = glm::normalize(other_end - spawn_point);
        spawn_point = road->outer_traffic_lane[random_index].position;
    }
    else
    {
        int random_index = rand() % road->inner_traffic_lane.size();
        int next_index = get_next_or_prev_index_from_vector(random_index, road->inner_traffic_lane, false);
        spawn_point = road->inner_traffic_lane[random_index].position;
        glm::vec3 other_end = road->inner_traffic_lane[next_index].position;
        if (glm::length(other_end - spawn_point) == 0)
        {
            next_index = get_next_or_prev_index_from_vector(next_index, road->inner_traffic_lane, false);
            other_end = road->inner_traffic_lane[next_index].position;
        }
        forward = glm::normalize(other_end - spawn_point);
        spawn_point = road->inner_traffic_lane[random_index].position;
    }
    player->spawn_player(spawn_point, forward);
}

void MainEvent::set_random_position_and_direction(Bot* bot)
{
    glm::vec3 spawn_point;
    glm::vec3 forward;
    if (rand() % 2 == 1)
    {
        int random_index = rand() % road->outer_traffic_lane.size();
        int next_index = get_next_or_prev_index_from_vector(random_index, road->outer_traffic_lane);
        spawn_point = road->outer_traffic_lane[random_index].position;
        glm::vec3 other_end = road->outer_traffic_lane[next_index].position;
        if (other_end == spawn_point)
        {
            next_index = get_next_or_prev_index_from_vector(next_index, road->outer_traffic_lane);
            other_end = road->outer_traffic_lane[next_index].position;
        }
        forward = glm::normalize(other_end - spawn_point);
        spawn_point = road->outer_traffic_lane[random_index].position;
        bot->current_waypoint_index = random_index;
        bot->to_next_waypoint = 1;
    }
    else
    {
        int random_index = rand() % road->inner_traffic_lane.size();
        int next_index = get_next_or_prev_index_from_vector(random_index, road->inner_traffic_lane, false);
        spawn_point = road->inner_traffic_lane[random_index].position;
        glm::vec3 other_end = road->inner_traffic_lane[next_index].position;
        if (other_end == spawn_point)
        {
            next_index = get_next_or_prev_index_from_vector(next_index, road->inner_traffic_lane, false);
            other_end = road->inner_traffic_lane[next_index].position;
        }
        forward = glm::normalize(other_end - spawn_point);
        bot->current_waypoint_index = random_index;
        spawn_point = road->inner_traffic_lane[random_index].position;
        bot->to_next_waypoint = -1;
    }
    bot->spawn_bot(spawn_point, forward);
}

void MainEvent::check_for_bot_next_waypoint(Bot* bot)
{
    int next_waypoint_index;
    int new_next_waypoint_index;
    glm::vec3 current_waypoint = bot->to_next_waypoint == 1 ? road->outer_traffic_lane[bot->current_waypoint_index].position
															: road->inner_traffic_lane[bot->current_waypoint_index].position;
    glm::vec3 next_waypoint;
    glm::vec3 new_next_waypoint;
    if (bot->to_next_waypoint == 1)
    {
        next_waypoint_index = get_next_or_prev_index_from_vector(bot->current_waypoint_index, road->outer_traffic_lane);
        next_waypoint = road->outer_traffic_lane[next_waypoint_index].position;
        new_next_waypoint_index = get_next_or_prev_index_from_vector(next_waypoint_index, road->outer_traffic_lane);
        new_next_waypoint = road->outer_traffic_lane[new_next_waypoint_index].position;
    }
    else 
    {
        next_waypoint_index = get_next_or_prev_index_from_vector(bot->current_waypoint_index, road->inner_traffic_lane, false);
        next_waypoint = road->inner_traffic_lane[next_waypoint_index].position;
        new_next_waypoint_index = get_next_or_prev_index_from_vector(next_waypoint_index, road->inner_traffic_lane, false);
        new_next_waypoint = road->inner_traffic_lane[new_next_waypoint_index].position;
    }
    if (glm::length(bot->position - current_waypoint) >= glm::length(next_waypoint - current_waypoint))
    {
    	bot->current_waypoint_index = next_waypoint_index;
    	glm::vec3 new_forward = glm::normalize(new_next_waypoint - bot->position);
        bot->spawn_bot(bot->position, new_forward);
    }
}

void MainEvent::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Camera* camera)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    int location = glGetUniformLocation(shader->GetProgramID(), "Model");

    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int viewLocation = glGetUniformLocation(shader->GetProgramID(), "View");

    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    int projectionLocation = glGetUniformLocation(shader->GetProgramID(), "Projection");

    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(camera->projectionMatrix));

    int car_position_location = glGetUniformLocation(shader->GetProgramID(), "car_position");

    // Trecerea din coordante lume in spatiul coordonatelor de dispozitiv normalizate
    glm::vec4 car_pos = camera->projectionMatrix * viewMatrix * player->model_matrix * glm::vec4(glm::vec3(0), 1);
    glUniform4fv(car_position_location, 1, glm::value_ptr(car_pos));

    int curvature_scale_factor_location = glGetUniformLocation(shader->GetProgramID(), "curvature_scale_factor");
    glUniform1f(curvature_scale_factor_location, curvature_scale_factor);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void MainEvent::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT) && !bind_camera_to_player)
    {
        float cameraSpeed = 2 * player->max_speed;

        if (window->KeyHold(GLFW_KEY_W)) {
            main_camera->TranslateForward(deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            main_camera->TranslateRight(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            main_camera->TranslateForward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            main_camera->TranslateRight(deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            main_camera->TranslateUpward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            main_camera->TranslateUpward(deltaTime * cameraSpeed);
        }
    } else {
        if (window->KeyHold(GLFW_KEY_W)) {
            player->accelerate(deltaTime);
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            player->accelerate(-deltaTime);
        }
        if (window->KeyHold(GLFW_KEY_A)) {
            player->turn_right(deltaTime);
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            player->turn_right(-deltaTime);
        }
        if (window->KeyHold(GLFW_KEY_EQUAL))
        {
            curvature_scale_factor += deltaTime / 30;
        }
        if (window->KeyHold(GLFW_KEY_MINUS))
        {
            curvature_scale_factor -= deltaTime / 30;
        }
    }
}


void MainEvent::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_B)
    {
        bind_camera_to_player = !bind_camera_to_player;
    }
    if(key == GLFW_KEY_P)
    {
        cout << "Player position: [" << player->position.x << " " << player->position.y << " " << player->position.z << "]" << endl;
    }
    if(key == GLFW_KEY_T)
    {
        show_road_outline = !show_road_outline;
    }
    if(key == GLFW_KEY_I)
    {
        curvature_scale_factor *= -1;
    }
    if (key == GLFW_KEY_R)
    {
        curvature_scale_factor = 0;
    }
}


void MainEvent::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void MainEvent::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT) && !bind_camera_to_player)
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            main_camera->RotateFirstPerson_OX(-deltaY * sensivityOX);
            main_camera->RotateFirstPerson_OY(-deltaX * sensivityOY);
        }
    }
}


void MainEvent::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void MainEvent::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void MainEvent::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void MainEvent::OnWindowResize(int width, int height)
{
}
