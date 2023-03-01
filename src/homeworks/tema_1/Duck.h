#pragma once

#ifndef __DUCK__
#define __DUCK__

#include "mainEvent.h"

constexpr int DUCK_STATE_ACTIVE = 0;
constexpr int DUCK_STATE_SHOT = 1;
constexpr int DUCK_STATE_EVADING = 2;
constexpr int DUCK_STATE_EVADED = 3;
constexpr int DUCK_STATE_DEAD = 4;

constexpr int WING_STATE_IN = 1;
constexpr int WING_STATE_OUT = 2;

namespace tema1
{
	class Duck
	{
	public:
		
		Duck(int resolutionX, int resolutionY);
		~Duck();

		std::unordered_map<Mesh*, glm::mat3> drawDuck();

		/**
		 * \brief Get all meshes of which duck is constructed
		 * \return meshes
		 */
		std::vector<Mesh*> getAllMeshes();

		/**
		 * \brief This method is responsible for moving the duck depending on its state. Can give it a random direction by a chance,
		 * increasing also time alive of the duck. This method also is responsible for rebounds from a wall. After moving, it updates the duck
		 * properties.
		 * \param deltaTimeSeconds time between last frame and current
		 * \param resolutionX window x resolution
		 * \param resolutionY window y resolution
		 * \return Map of all meshes and their modelMatrices (calling the drawDuck method)
		 */
		std::unordered_map<Mesh*, glm::mat3> moveDuck(float deltaTimeSeconds, float resolutionX, float resolutionY);

		/**
		 * \brief Updating duck properties after changing its tailScale, direction or something else.
		 */
		void updateDuckProperties();

		/**
		 * \brief Makes a duck flies, in other words, it is responsible for the wing state and angle.
		 * \param deltaTimeSeconds time between last frame and current
		 */
		void fly(float deltaTimeSeconds);

		/**
		 * \brief Get the beak peak real coordinates, based on unrotated beak coordinates and its direction
		 * \return vec2 with x and y coordinates
		 */
		glm::vec2 getBeakPeakRealCoordinates();

		/**
		 * \brief get the duck size on Ox and Oy axis
		 * \return vec2 with x and y size (width and height)
		 */
		glm::vec2 getSize();

		/**
		 * \brief get trigonometric circle quadrant in which direction angle is situated
		 * \return trigonometric circle quadrant index
		 */
		int getCircleQuadrant();

		/**
		 * \brief Checks if a duck collides with a point
		 * \param collisionX collision x coord
		 * \param collisionY collision y coord
		 * \return true - collision, false otherwise
		 */
		bool Duck::checkForCollision(int collisionX, int collisionY);

		//// GETTERS
		int getDuckState();
		float getScoreForDuck();
		float getDefaultSpeed();
		float getDefaultScoreForDuck();
		float getDirection();
		float getSpeed();

		bool getDrawHitbox();

		float getTailX();
		float getTailY();
		float getTailScaleX();
		float getTailScaleY();
		float getDefaultWingAngle();
		float getDefaultTailScale();

		//float getBodyX();
		//float getBodyY();
		//float getBodyScaleX();
		//float getBodyScaleY();
		//int getWingAngle();
		//float getWingScaleX();
		//float getWingScaleY();
		//float getLeftWingX();
		//float getLeftWingY();
		//float getRightWingX();
		//float getRightWingY();
		//float getBeakX();
		//float getBeakY();
		//float getBeakScaleX();
		//float getBeakScaleY();
		//float getHeadX();
		//float getHeadY();
		//float getHeadScaleX();
		//float getHeadScaleY();

		//// SETTERS
		void setDuckState(int duckState);
		void setScoreForDuck(float scoreForDuck);
		void setDefaultSpeed(float defaultSpeed);
		void setDefaultScoreForDuck(float defaultScoreForDuck);
		void setDirection(float direction);
		void setSpeed(float speed);
		
		void setDrawHitbox(bool drawHitbox);

		void setTailX(float tailX);
		void setTailY(float tailY);
		void setTailScaleX(float tailScaleX);
		void setTailScaleY(float tailScaleY);
		void setDefaultWingAngle(float defaultWingAngle);
		void setDefaultTailScale(float defaultTailScale);

		//void setBodyX(float bodyX);
		//void setBodyY(float bodyY);
		//void setBodyScaleX(float bodyScaleX);
		//void setBodyScaleY(float bodyScaleY);
		//void setWingAngle(float wingAngle);
		//void setWingScaleX(float wingScaleX);
		//void setWingScaleY(float wingScaleY);
		//void setLeftWingX(float leftWingX);
		//void setLeftWingY(float leftWingY);
		//void setRightWingX(float rightWingX);
		//void setRightWingY(float rightWingY);
		//void setHeadX(float headX);
		//void setHeadY(float headY);
		//void setHeadScaleX(float headScaleX);
		//void setHeadScaleY(float headScaleY);
		//void setBeakX(float beakX);
		//void setBeakY(float beakY);
		//void setBeakScaleX(float beakScaleX);
		//void setBeakScaleY(float beakScaleY);
		

	private:
		Mesh* duckBody;
		Mesh* duckTail;
		Mesh* duckHead;
		Mesh* duckBeak;
		Mesh* duckLeftWing;
		Mesh* duckRightWing;
		Mesh* hitbox;

		float defaultSpeed = 450;
		int defaultWingAngle = 15;
		float defaultScoreForDuck = 1;
		
		float defaultTailScale;

		float timeAlive;
		float directionChangerCount;

		bool drawHitbox;

		float bodyX, bodyY, bodyScaleX, bodyScaleY;
		float tailX, tailY, tailScaleX, tailScaleY;
		float wingScaleX, wingScaleY, leftWingX, leftWingY, rightWingX, rightWingY;
		float headScaleX, headScaleY, headX, headY;
		float beakScaleX, beakScaleY, beakX, beakY;
		float wingAngle;
		int wingState;
		int duckState;
		float scoreForDuck;
		float direction;
		float speed;
		
	};
}
	

#endif