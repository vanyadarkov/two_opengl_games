#pragma once

#include "components/simple_scene.h"
#include "homeworks/tema_2/camera.h"
#include "homeworks/tema_2/Road.h"
#include "homeworks/tema_2/Player.h"
#include "homeworks/tema_2/Terrain.h"
#include "homeworks/tema_2/Bot.h"
#include "homeworks/tema_2/Tree.h"
#include "homeworks/tema_2/utils.h"

const glm::vec3 road_color = NormalizedRGB(37, 23, 73);
const glm::vec3 sky_color = NormalizedRGB(130, 195, 236);
const glm::vec3 terrain_color = NormalizedRGB(40, 84, 48);

const vector<glm::vec3> trunk_colors = { NormalizedRGB(85, 57, 57), NormalizedRGB(115, 95, 50), NormalizedRGB(102, 90, 72),
										NormalizedRGB(170, 139, 86), NormalizedRGB(159, 135, 114), NormalizedRGB(60, 35, 23)};

const vector<glm::vec3> crown_colors = { NormalizedRGB(130, 148, 96), NormalizedRGB(28, 103, 88), NormalizedRGB(127, 183, 126),
                                        NormalizedRGB(5, 113, 83), NormalizedRGB(34, 75, 12), NormalizedRGB(89, 206, 143) };

constexpr float DISTANCE_TO_TARGET = 2.5f;
constexpr int OUT_OF_ROAD_TREES = 1000;
constexpr int NUMBER_OF_BOTS = 50;

namespace tema2
{
    class MainEvent : public gfxc::SimpleScene
    {
     public:
    	MainEvent();
        ~MainEvent();

        void Init() override;

     private:
        /**
         * \brief Seteaza pozitie random pentru player pe o anumita banda de circulatie cu vectorul de directie specific si spauneaza playerul.
         * \param player jucatorul pe care il spaunam
         */
        void set_random_position_and_direction(Player * player);

        /**
         * \brief Seteaza pozitie random pentru bot pe o anumita banda de circulatie cu vectorul de directie specific. De asemenea, seteaza indexul
         * waypointului curent al botului pe traseu si operatia necesara pentru a obtine urmatorul index. 
         * \param bot botul pentru spaunare
         */
        void set_random_position_and_direction(Bot* bot);

        /**
         * \brief Verifica daca botul a ajuns la urmatorul waypoint al traseului si il seteaza in caz de necesitate
         * \param bot bot-ul pentru care verificam
         */
        void check_for_bot_next_waypoint(Bot * bot);

        /**
         * \brief Randeaza scena in camera
         * \param camera camera in care randam scena
         */
        void render_scene(Camera* camera);

        /**
         * \brief Spaunare copaci in scena. Se spauneaza de-a lungul drumului pe interior si exterior, de asemenea
         * la pozitii random pe tot terenul (un numar de OUT_OF_ROAD_TREES)
         */
        void spawn_trees();

        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        /**
         * \brief Randeaza un mesh in scena
         * \param mesh mesh-ul pe care renderim
         * \param shader shader-ul aplicat
         * \param modelMatrix matricea de modelare a meshului
         * \param camera camera din care extragem matricile de proiectie si vizualizare
         */
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Camera* camera);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        /**
         * \brief Executia calculelor de coliziune a playerului cu botii si miscare botilor si playerilor
         * \param deltaTimeSeconds frame time
         */
        void do_calculations(float deltaTimeSeconds);

        /**
         * \brief Randare scena in viewportul principal
         */
        void render_main_viewport();

        /**
         * \brief Randare scena in viewportul pentru mini mapa.
         */
        void render_mini_map_viewport();

     protected:
    	Camera* main_camera;
        Camera* mini_map_camera;

        int first_frame = 1;

        // PERSPECTIVE
        float fov;
        float aspectRatio;

        // ORTHO
        float left;
        float right;
        float bottom;
        float top;

        bool show_road_outline;

        float zNear;
        float zFar;

        float car_max_width;
        float car_max_height;
        float car_max_length;

        float curvature_scale_factor;

        bool bind_camera_to_player;

        tema2::Road* road;
        tema2::Player* player;
        tema2::Terrain* terrain;
        vector<Bot*> other_bots;

        vector<Tree*> trees;

        int number_of_bots;
        Mesh* middleMeshCurve;
        Mesh* innerMeshCurve;
        Mesh* outerMeshCurve;

        Mesh* inner_lane_mesh;
        Mesh* outer_lane_mesh;

        Mesh* roadMesh;
        Mesh* player_car;

        Shader* shader;
    };
}