#include "homeworks/tema_2/utils.h"

namespace tema2
{
	class Tree
	{
	public:
		/**
		 * \brief Construieste un copac
		 * \param name numele meshului pentru copacul curent
		 * \param trunk_scale factorul de scalare a tulpinii
		 * \param crown_scale factorul de scalare a coroanei copacului
		 * \param position pozitia la care il spaunam
		 * \param tree_trunk_color culoarea tulpinii
		 * \param tree_crown_color culoarea coroanei
		 */
		Tree(std::string name, glm::vec3 trunk_scale, glm::vec3 crown_scale, glm::vec3 position, glm::vec3 tree_trunk_color, glm::vec3 tree_crown_color);
		~Tree();

		void update_model_matrices();
		void create_tree_meshes();

		std::string name;
		glm::vec3 trunk_scale;
		glm::vec3 crown_scale;
		glm::vec3 position;

		glm::mat4 tree_crown_model_matrix;
		glm::mat4 tree_trunk_model_matrix;

		glm::vec3 tree_crown_color;
		glm::vec3 tree_trunk_color;
		Mesh* tree_trunk_mesh;
		Mesh* tree_crown_mesh;
	};
}
