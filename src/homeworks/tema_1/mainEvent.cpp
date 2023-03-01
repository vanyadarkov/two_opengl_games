#include "homeworks/tema_1/mainEvent.h"
#include "homeworks/tema_1/Duck.h"



using namespace std;
using namespace tema1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


MainEvent::MainEvent()
{
    respawnCooldown = 0;
    numberOfBullets = defaultNumberOfBullets;
    numberOfLives = defaultNumberOfLives;
    resolution = window->GetResolution();
    duck = new Duck(resolution.x, resolution.y);

    heart_filled = object2D::CreateHeart("heart_filled", left_bottom_corner, 1, color_heart, true);
    heart_empty = object2D::CreateHeart("heart_empty", left_bottom_corner, 1, color_heart, false);
    bullet_filled = object2D::CreateBullet("bullet_filled", left_bottom_corner, 1, color_gold, true);
    bullet_empty = object2D::CreateBullet("bullet_empty", left_bottom_corner, 1, color_gold, false);
    grass = object2D::CreateSquare("grass", left_bottom_corner, 1, color_grass, true);

    textRendererGameStatus = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
    textRendererGame = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
    textRendererAuxiliar = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);

    fontSizeGameStatus = resolution.y / 40; 
    fontSizeGame = resolution.y / 10;
    fontSizeAuxiliar = resolution.y / 35;

    gameState = GAME_STATE_MENU;
    drawCrosshair = false;
    
    cursorX = 0;
    cursorY = 0;
}


MainEvent::~MainEvent()
{
}


void MainEvent::Init()
{
    resolution = window->GetResolution();

    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 1));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    textRendererGameStatus->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Upheaval.ttf"), fontSizeGameStatus);
    textRendererGame->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Upheaval.ttf"), fontSizeGame);
    textRendererAuxiliar->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Upheaval.ttf"), fontSizeAuxiliar);
   
    resetGame();

    AddMeshToList(heart_filled);
    AddMeshToList(heart_empty);
    AddMeshToList(bullet_filled);
    AddMeshToList(bullet_empty);
    AddMeshToList(grass);

    for (Mesh* mesh : duck->getAllMeshes()) {
        AddMeshToList(mesh);
    }
    srand(time(0));
}

void MainEvent::FrameStart()
{
    resolution = window->GetResolution();
    
    static glm::vec3 backgroundColor = color_sky;
    if (gameState == GAME_STATE_MENU)
        backgroundColor = color_sky;
    else if (gameState == GAME_STATE_OVER)
        backgroundColor = color_game_over;
    else if (gameState == GAME_STATE_ACTIVE || gameState == GAME_STATE_PAUSED || gameState == GAME_STATE_RESUMING) {
        if (duck->getDuckState() == DUCK_STATE_EVADED || duck->getDuckState() == DUCK_STATE_EVADING)
            backgroundColor = color_evaded;
        if (duck->getDuckState() == DUCK_STATE_ACTIVE)
            backgroundColor = color_sky;
        if (duck->getDuckState() == DUCK_STATE_SHOT || duck->getDuckState() == DUCK_STATE_DEAD)
            backgroundColor = color_hit;
    }

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, resolution.x, resolution.y);

    if (gameState != GAME_STATE_OVER && gameState != GAME_STATE_MENU) {
        drawCrosshair = true;
        window->HidePointer();
    }
    else {
        drawCrosshair = false;
        window->ShowPointer();
    }

    if (gameState == GAME_STATE_ACTIVE) {
        if (bestScore < score) bestScore = score;
        if (ducksShot == 5 && difficulty < maxDifficulty) {
            ducksShot = 0;
            difficulty++;
            duck->setScoreForDuck(duck->getScoreForDuck() * 2);
            duck->setSpeed(duck->getDefaultSpeed() + (difficulty - 1) * duck->getDefaultSpeed() / 5);
            float tailScale = duck->getTailScaleX() / 1.1f;
            duck->setTailScaleX(tailScale);
            duck->updateDuckProperties();
        }
    }
}

void MainEvent::Update(float deltaTimeSeconds)
{   
    if (drawCrosshair) {
        drawCursor();
    }
    if (gameState == GAME_STATE_MENU) {
        drawMenu();
    }
    else if (gameState == GAME_STATE_STARTING) {
        startGame(deltaTimeSeconds);
    }
    else {
        drawGameStatus(deltaTimeSeconds);
        drawGrass();
        if (gameState == GAME_STATE_OVER)
        {
            drawGameOver();
        }
        else if (gameState == GAME_STATE_ACTIVE) {
            if (duck->getDuckState() == DUCK_STATE_EVADED || duck->getDuckState() == DUCK_STATE_DEAD) {
                // respawn duck check
                if (respawnCooldown >= 1) {
                    if (duck->getDuckState() == DUCK_STATE_EVADED) {
                        if (numberOfLives > 0) numberOfLives--;
                        else gameState = GAME_STATE_OVER;
                    }
                    numberOfBullets = defaultNumberOfBullets;
                    duck->setDuckState(DUCK_STATE_ACTIVE);
                    duckFirstSpawn = true;
                    respawnCooldown = 0;
                }
                else {
                    respawnCooldown += deltaTimeSeconds;
                }
            }
            if (duckFirstSpawn) {
                duckFirstSpawn = false;
                duck->setDuckState(DUCK_STATE_ACTIVE);
                generateRandomPosition();
                duck->updateDuckProperties();
            }
        }
        if(duck->getDuckState() != DUCK_STATE_EVADED && duck->getDuckState() != DUCK_STATE_DEAD) 
            renderDuck(deltaTimeSeconds);
    }    

}

void MainEvent::FrameEnd()
{
    
}

void MainEvent::drawCursor() {
    static Mesh* crosshairRectangle;
    if (!crosshairRectangle) {
        crosshairRectangle = object2D::CreateSquare("crosshairRectangle", left_bottom_corner, 1, color_crosshair, true);
        AddMeshToList(crosshairRectangle);
    }

    float scaleX = round(resolution.x / 128.0f);
    float scaleY = scaleX / 2;
    float offset = round(scaleX / 4);
    
    // left rectangle
    glm::mat3 modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(cursorX - scaleX - offset, cursorY);
    modelMatrix *= transform2D::Translate(0, -offset);
    modelMatrix *= transform2D::Scale(scaleX, scaleY);
    RenderMesh2D(crosshairRectangle, shaders["VertexColor"], modelMatrix);

    // right rectangle
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(cursorX + offset, cursorY);
    modelMatrix *= transform2D::Translate(0, -offset);
    modelMatrix *= transform2D::Scale(scaleX, scaleY);
    RenderMesh2D(crosshairRectangle, shaders["VertexColor"], modelMatrix);

    // bottom rectangle
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(cursorX, cursorY - scaleX - offset);
    modelMatrix *= transform2D::Translate(-offset, 0);
    modelMatrix *= transform2D::Scale(scaleY, scaleX);
    RenderMesh2D(crosshairRectangle, shaders["VertexColor"], modelMatrix);

    // upper rectangle
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(cursorX, cursorY + offset);
    modelMatrix *= transform2D::Translate(-offset, 0);
    modelMatrix *= transform2D::Scale(scaleY, scaleX);
    RenderMesh2D(crosshairRectangle, shaders["VertexColor"], modelMatrix);
}

void MainEvent::drawGameOver() {
    string gameOver = "Game Over :(";
    float gameOverPosX = ((gameOver.size() - 3) * fontSizeGame) / 2;
    float gameOverPosY = 2.5f * fontSizeGame;
    textRendererGame->RenderText(gameOver, gameOverPosX, gameOverPosY, 1, color_text);

    string bestScoreString = "Best score: " + std::to_string(bestScore);
    float bestScorePosX = resolution.x / 2 - (bestScoreString.size() * fontSizeAuxiliar) / 2;
    float bestScorePosY = gameOverPosY + fontSizeGame / 2 + fontSizeAuxiliar * 1.5f;
    textRendererAuxiliar->RenderText(bestScoreString, bestScorePosX, bestScorePosY, 1, color_text);

    string goToMenu = "Press M for menu";
    float goToMenuPosX = gameOverPosX;
    float goToMenuPosY = 4 * fontSizeGame;
    textRendererAuxiliar->RenderText(goToMenu, goToMenuPosX, goToMenuPosY, 1, color_text);

    string startGame = "Press P to start game";
    float startGamePosX = goToMenuPosX;
    float startGamePosY = goToMenuPosY + fontSizeAuxiliar;
    textRendererAuxiliar->RenderText(startGame, startGamePosX, startGamePosY, 1, color_text);
}

void MainEvent::resetGame() {
    duck->setScoreForDuck(1);
    duck->setSpeed(resolution.x / 2.8f);
    duck->setDefaultSpeed(resolution.x / 2.8f);
    duck->setDefaultScoreForDuck(1);
    duck->setDefaultWingAngle(15);
    duck->setDefaultTailScale(resolution.x / 51);
    duck->setTailScaleX(duck->getDefaultTailScale());

    duck->updateDuckProperties();

    duckFirstSpawn = true;
    respawnCooldown = 0;
    numberOfBullets = defaultNumberOfBullets;
    numberOfLives = defaultNumberOfLives;
    score = 0;
    difficulty = 1;
    ducksShot = 0;
}

void MainEvent::drawMenu() {
    string gameTitle = "Duck Hunt";
    float titlePosX = (gameTitle.size() * fontSizeGame);
    float titlePosY = 1.5 * fontSizeGame;
    textRendererGame->RenderText(gameTitle, titlePosX / 2, titlePosY, 1, color_text);

    string description = "from AliExpress";
    float descriptionPosX = resolution.x - (description.size() * 2 * fontSizeAuxiliar);
    float descriptionPosY = titlePosY + fontSizeGame;
    textRendererAuxiliar->RenderText(description, descriptionPosX * 1.2f, descriptionPosY, 1, color_text);

    string toPlay = "Press p to play";
    float toPlayPosX = (toPlay.size() * fontSizeAuxiliar);
    float toPlayPosY = resolution.y / 2;
    textRendererAuxiliar->RenderText(toPlay, toPlayPosX, toPlayPosY, 1, color_text);

    string toPause = "In game, press p to pause/resume"; 
    float toPausePosX = toPlayPosX;
    float toPausePosY = toPlayPosY + 2 * fontSizeAuxiliar;
    textRendererAuxiliar->RenderText(toPause, toPausePosX, toPausePosY, 1, color_text);

    string toQuit = "Press ESC to quit";
    float toQuitPosX = toPlayPosX;
    float toQuitPosY = toPlayPosY + fontSizeAuxiliar;
    textRendererAuxiliar->RenderText(toQuit, toQuitPosX, toQuitPosY, 1, color_text);
}

void MainEvent::startGame(float deltaTimeSeconds) {
    static float starting = 3;
    if (starting <= 0) {
        gameState = GAME_STATE_ACTIVE;
        starting = 3;
        resetGame();
    }
    else {
        starting -= deltaTimeSeconds;
        string remaining = std::to_string((int)std::ceil(starting));
        float posX = resolution.x / 2 - fontSizeGame / 2;
        float posY = resolution.y / 2 - fontSizeGame;
        textRendererGame->RenderText(remaining, posX, posY, 1, color_text);
    }
}

void MainEvent::renderDuck(float deltaTimeSeconds) {
    glm::ivec2 resolution = window->GetResolution();
    if (gameState == GAME_STATE_ACTIVE) {
        for (auto& meshToRender : duck->moveDuck(deltaTimeSeconds, window->GetResolution().x, window->GetResolution().y)) {
            RenderMesh2D(meshToRender.first, shaders["VertexColor"], meshToRender.second);
        }
    }
    else if (gameState == GAME_STATE_PAUSED || gameState == GAME_STATE_RESUMING) {
        for (auto& meshToRender : duck->drawDuck()) {
            RenderMesh2D(meshToRender.first, shaders["VertexColor"], meshToRender.second);
        }
    }
}

void MainEvent::drawGameStatus(float deltaTimeSeconds) {
    
    float offsetX = window->GetResolution().x / 90;
    float offsetY = window->GetResolution().y / 45;
    float iconScale = duck->getSize().y - 3 * offsetY;
    iconScale /= 2;

    float xPos = iconScale;
    float yPos = duck->getSize().y - iconScale - offsetY;

    glm::mat3 modelMatrix = glm::mat3(1);
    Mesh* meshToRender;

    int filled = numberOfLives;
    for (int i = 0; i < defaultNumberOfLives; i++) {
        if (filled > 0) {
            filled--;
            meshToRender = heart_filled;
        }
        else {
            meshToRender = heart_empty;
        }
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(xPos + (offsetX + iconScale) * i, yPos);
        modelMatrix *= transform2D::Scale(iconScale, iconScale);
        RenderMesh2D(meshToRender, shaders["VertexColor"], modelMatrix);
    }

    filled = numberOfBullets;

    for (int i = 0; i < defaultNumberOfBullets; i++) {
        if (filled > 0) {
            filled--;
            meshToRender = bullet_filled;
        }
        else {
            meshToRender = bullet_empty;
        }
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(xPos + (offsetX + iconScale) * i, yPos - iconScale - offsetY);
        modelMatrix *= transform2D::Scale(iconScale, iconScale);
        RenderMesh2D(meshToRender, shaders["VertexColor"], modelMatrix);
    }

    string scoreString = "Score: " + std::to_string(score);
    int scoreStringSize = scoreString.size();
    textRendererGameStatus->RenderText(scoreString, offsetX, offsetY, 1, color_text);
    string difficultyString = "Difficulty: " + std::to_string(difficulty);
    textRendererGameStatus->RenderText(difficultyString, offsetX, offsetY + fontSizeGameStatus + offsetY, 1, color_text);


    if (gameState == GAME_STATE_PAUSED || gameState == GAME_STATE_RESUMING) {
        string pause = "Paused...";
        static float flicker = 2;
        float posX = resolution.x - (pause.size() - 1) * fontSizeGameStatus;
        float posY = offsetY;
        if (flicker >= 1) {
            textRendererGameStatus->RenderText(pause, posX, posY, 1, color_text);
            flicker -= deltaTimeSeconds;
        }
        else if (flicker < 1 && flicker >= 0) {
            flicker -= deltaTimeSeconds;
        } 
        else if (flicker < 0) {
            flicker = 2;
        }
        if (gameState == GAME_STATE_RESUMING) {
            static float resumingIn = 3;
            if (resumingIn <= 0) {
                gameState = GAME_STATE_ACTIVE;
                resumingIn = 3;
            }
            else {
                resumingIn -= deltaTimeSeconds;
                string resuming = "Resuming in " + std::to_string((int)ceil(resumingIn)) + "...";
                float resPosX = resolution.x - (resuming.size() - 3) * fontSizeGameStatus;
                float resPosY = offsetY + fontSizeGameStatus;
                textRendererGameStatus->RenderText(resuming, resPosX, resPosY, 1, color_text);
            } 
        }
    }
}

void MainEvent::drawGrass() {
    int scaleX = window->GetResolution().x;
    int scaleY = duck->getSize().y;

    glm::mat3 modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Scale(scaleX, (float) scaleY);
    RenderMesh2D(grass, shaders["VertexColor"], modelMatrix);
}


void MainEvent::generateRandomPosition() {
    bool generate = true;
    float randX;
    float randY = 0;
    float randDirection;
    while (generate) {
        randX = rand() % (window->GetResolution().x - 2 * (int)duck->getSize().y) + 2 * duck->getSize().y;
        randDirection = rand() % 180;
        // check if direction angle is far enough from the extremities (90, 180 , 270, 360)
        if (abs(sin(RADIANS(randDirection))) <= 0.8 && abs(cos(RADIANS(randDirection))) <= 0.8)
            generate = false;
    }
    
    duck->setTailX(randX);
    duck->setTailY(randY);
    duck->setDirection(randDirection);
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void MainEvent::OnInputUpdate(float deltaTime, int mods)
{
    // For testing, key up and down increase / decrease
    if (window->KeyHold(GLFW_KEY_UP)) {
        if(gameState == GAME_STATE_ACTIVE)
            duck->setDirection(duck->getDirection() + RADIANS(50 * deltaTime));
    }
    if (window->KeyHold(GLFW_KEY_DOWN)) {
        if (gameState == GAME_STATE_ACTIVE)
            duck->setDirection(duck->getDirection() - RADIANS(50 * deltaTime));
    }
}


void MainEvent::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_M) {
        if (gameState == GAME_STATE_OVER) gameState = GAME_STATE_MENU;
    }
    if (key == GLFW_KEY_H) {
        if (gameState == GAME_STATE_ACTIVE) duck->setDrawHitbox(!duck->getDrawHitbox());
    }
    if (key == GLFW_KEY_P) {
        if (gameState == GAME_STATE_ACTIVE) gameState = GAME_STATE_PAUSED;
        else if (gameState == GAME_STATE_MENU || gameState == GAME_STATE_OVER) gameState = GAME_STATE_STARTING;
        else if (gameState == GAME_STATE_PAUSED) gameState = GAME_STATE_RESUMING;
    }
}


void MainEvent::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void MainEvent::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    cursorX = mouseX;
    cursorY = window->GetResolution().y - mouseY;
    
}


void MainEvent::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (gameState != GAME_STATE_ACTIVE) {
        return;
    }

    // Shoot the duck. Only works if duck is in active state.
    if (duck->getDuckState() == DUCK_STATE_ACTIVE) {
        float mouseRealX = mouseX;
        float mouseRealY = window->GetResolution().y - mouseY;
        if (numberOfBullets > 0) {
            numberOfBullets--;
            if (duck->checkForCollision(mouseRealX, mouseRealY)) {
                score += duck->getScoreForDuck();
                ducksShot++;
                duck->setDuckState(DUCK_STATE_SHOT);
            }
        }
    }
    
    
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
    // Constant window resolution x and y
    window->SetSize(resolution.x, resolution.y);
}
