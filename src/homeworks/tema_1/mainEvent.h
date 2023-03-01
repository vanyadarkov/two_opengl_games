#pragma once

#ifndef __MAIN_EVENT__
#define __MAIN_EVENT__

#include "components/simple_scene.h"
#include "components/text_renderer.h"
#include "homeworks/tema_1/transform2D.h"
#include "homeworks/tema_1/object2D.h"
#include "homeworks/tema_1/Duck.h"

#include <vector>
#include <iostream>

constexpr int GAME_STATE_MENU = 0;
constexpr int GAME_STATE_STARTING = 1;
constexpr int GAME_STATE_ACTIVE = 2;
constexpr int GAME_STATE_PAUSED = 3;
constexpr int GAME_STATE_RESUMING = 4;
constexpr int GAME_STATE_OVER = 5;

const glm::vec3 color_black = NormalizedRGB(60 , 64, 72);
const glm::vec3 color_gray = NormalizedRGB (128, 128, 128);
const glm::vec3 color_green = NormalizedRGB(0, 171, 179);
const glm::vec3 color_orange = NormalizedRGB(220, 95, 0);

const glm::vec3 color_game_over = NormalizedRGB(167, 0, 0);

const glm::vec3 color_text = NormalizedRGB(0, 0, 0);

const glm::vec3 color_sky = NormalizedRGB(135, 206, 235);
const glm::vec3 color_evaded = NormalizedRGB(69, 85, 102);
const glm::vec3 color_hit = NormalizedRGB(135, 236, 209);

const glm::vec3 color_heart = NormalizedRGB(227, 27, 35);
const glm::vec3 color_gold = NormalizedRGB(255, 215, 0);

const glm::vec3 color_grass = NormalizedRGB(126, 200, 80);

const glm::vec3 color_crosshair = NormalizedRGB(0, 8, 193);

const glm::vec3 left_bottom_corner = { 0, 0, 0 };

namespace tema1
{

    class MainEvent : public gfxc::SimpleScene
    {
    public:
        MainEvent();
        ~MainEvent();
        

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        /**
         * \brief Game reset, game state, score, bullets, lives, duck states.
         */
        void resetGame();

        /**
         * \brief drawing the game status while playing (like score, difficulty, lives and bullets count, pause/resuming notification)
         * \param deltaTimeSeconds frame time
         */
        void drawGameStatus(float deltaTimeSeconds);

        /**
         * \brief draw the game over screen.
         */
        void drawGameOver();

        /**
         * \brief Draw the grass rectangle behind which the duck is spawned
         */
        void drawGrass();

        /**
         * \brief Draw the menu screen (play, exit, game title...)
         */
        void drawMenu();

        /**
         * \brief Draw game start countdown
         * \param deltaTimeSeconds frame time
         */
        void startGame(float deltaTimeSeconds);

        /**
         * \brief Generating random position and rotation for duck respawn
         */
        void generateRandomPosition();

        /**
         * \brief Render the duck based on game state (ACTIVE / PAUSED / RESUMING)
         * \param deltaTimeSeconds frame time
         */
        void renderDuck(float deltaTimeSeconds);

        /**
         * \brief Draw crosshair on cursor position (if cursor is hidden)
         */
        void drawCursor();

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        // GUI icons
        Mesh* heart_filled;
        Mesh* heart_empty;
        Mesh* bullet_filled;
        Mesh* bullet_empty;
        Mesh* grass;
        // Duck
        Duck* duck;
        int gameState;

        // Renderer for text, 1 for each type of displayed text
        gfxc::TextRenderer* textRendererGameStatus;
        gfxc::TextRenderer* textRendererGame;
        gfxc::TextRenderer* textRendererAuxiliar;

        bool duckFirstSpawn = true;

        bool drawCrosshair;

        float cursorX;
        float cursorY;

        float respawnCooldown;
        glm::ivec2 resolution;
        int numberOfBullets;
        int defaultNumberOfBullets = 3;
        int numberOfLives;
        int defaultNumberOfLives = 3;
        int score = 0;
        int bestScore = 0;
        int fontSizeGameStatus;
        int fontSizeGame;
        int fontSizeAuxiliar;
        int difficulty = 1;
        int maxDifficulty = 8;
        int ducksShot = 0;
    };
}

#endif
