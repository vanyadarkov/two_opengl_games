#pragma once

#include "homeworks/tema_2/utils.h"
#include "homeworks/tema_2/Road.h"

using namespace tema2_utils;
using namespace std;

namespace tema2
{
	class Player
	{
	public:
		/**
		 * \brief 
		 * \param name numele meshului
		 * \param scale scale pentru meshul playerului
		 * \param color culoarea mashinii
		 * \param road drumul pe care playerul merge
		 */
		Player(std::string name, glm::vec3 scale, glm::vec3 color, Road * road);
		~Player();

		/**
		 * \brief Spauneaza un player
		 * \param position pozitia de spawn
		 * \param forward vectorul forward (inainte)
		 */
		void spawn_player(glm::vec3 position, glm::vec3 forward);

		/**
		 * \brief Face update la model matrix-ul masinii playerului
		 */
		void update_model_matrix();

		/**
		 * \brief Similar ca la bot, obtine coordonatele celor 4 punct a paralelogramului ce reprezinta masina jucatorului
		 * \return vector cu punctele masinii
		 */
		vector<glm::vec3> get_player_car_corners();

		/**
		 * \brief Opreste jucatorul
		 */
		void stop();

		/**
		 * \brief Misca masina jucatorului
		 * \param deltaTime frame time folosit la calcului pozitiei si rotatiei
		 */
		void move(float deltaTime);

		/**
		 * \brief Modifica pozitia inainte (forward)
		 * \param deltaTime frame time
		 */
		void move_forward(float deltaTime);
		/**
		 * \brief Accelereaza playerul
		 * \param deltaTime frame time
		 */
		void accelerate(float deltaTime);

		/**
		 * \brief Modifica vectorul forward in dependenta de deltaTime si current_rotation_angle
		 * \param deltaTime frame time
		 */
		void move_right(float deltaTime);

		/**
		 * \brief Mareste rotatia masinii (ungiul de rotatie al rotilor doar, cum ar veni)
		 * \param deltaTime frame time
		 */
		void turn_right(float deltaTime);

		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 forward = glm::normalize(glm::vec3(1, 0, 0));

		float current_speed;
		float max_speed;

		float direction_angle;

		bool accelerating = false;
		bool turning_right = false;

		float max_wheels_rotation_angle;
		float current_wheels_rotation_angle;
		glm::mat4 model_matrix;
		Mesh* player_mesh;

	private:
		
		Road* road;
		glm::vec3 color;
		
	};
}