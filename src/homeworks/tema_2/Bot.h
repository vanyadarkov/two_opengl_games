#pragma once

#include "homeworks/tema_2/utils.h"

using namespace std;
using namespace tema2_utils;

namespace tema2
{

	class Bot
	{
	public:
		Bot(std::string name, glm::vec3 scale, glm::vec3 color);
		~Bot();

		/**
		 * \brief Spauneaza botul la o pozitie si cu un vector de directie forward anumit
		 * \param position pozitia la care se spauneaza
		 * \param forward vectorul de directie forward (inainte)
		 */
		void spawn_bot(glm::vec3 position, glm::vec3 forward);

		/**
		 * \brief Updateaza model matrixul
		 */
		void update_model_matrix();

		/**
		 * \brief misca botul 
		 * \param deltaTime timpul de la frame-ul trecut la curent (calculare pozitie noua)
		 */
		void move(float deltaTime);

		/**
		 * \brief Coordonatele celor 4 colturi a masinii botului (ia in considerare rotatia)
		 * \return vector cu punctele celor 4 colturi
		 */
		vector<glm::vec3> get_bot_car_corners();

		/**
		 * \brief Misca botul inainte
		 * \param deltaTime frame time (calculare pozitie noua)
		 */
		void move_forward(float deltaTime);

		/**
		 * \brief accelereaza botul
		 * \param deltaTime frame time folosit la calcularea noii pozitii
		 */
		void accelerate(float deltaTime);

		/**
		 * \brief Opreste botul din miscare
		 */
		void stop();

		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 forward = glm::normalize(glm::vec3(1, 0, 0));

		float current_speed;
		float max_speed;

		bool accelerating = false;
		bool turning_right = false;

		float direction_angle;

		// indexul waypointului curent (de la care a plecat)
		int current_waypoint_index;
		// operatia aplicata asupra current waypoint index pentru a obtine urmatorul waypoint (+1 sau -1)
		int to_next_waypoint;

		Mesh* bot_mesh;
		glm::vec3 color;
		glm::mat4 model_matrix;
	};
}