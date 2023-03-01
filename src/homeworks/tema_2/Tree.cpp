#include "homeworks/tema_2/Tree.h"

using namespace tema2;

Tree::Tree(std::string name, glm::vec3 trunk_scale, glm::vec3 crown_scale, glm::vec3 position, glm::vec3 tree_trunk_color, glm::vec3 tree_crown_color)
{
	this->name = name;
	this->trunk_scale = trunk_scale;
	this->crown_scale = crown_scale;
	this->position = position;
	this->tree_crown_color = tree_crown_color;
	this->tree_trunk_color = tree_trunk_color;
	create_tree_meshes();
	update_model_matrices();
}

void Tree::create_tree_meshes()
{
	tree_trunk_mesh = tema2_utils::create_simple_cube(name + "_trunk", tree_trunk_color);
	tree_crown_mesh = tema2_utils::create_simple_cube(name + "_crown", tree_crown_color);
}


void Tree::update_model_matrices()
{
	glm::vec3 trunk_pos = position;
	tree_trunk_model_matrix = glm::mat4(1);
	tree_trunk_model_matrix = glm::translate(tree_trunk_model_matrix, trunk_pos);

	tree_trunk_model_matrix = glm::translate(tree_trunk_model_matrix, glm::vec3(-trunk_scale.x / 2, 0, -trunk_scale.z / 2));
	tree_trunk_model_matrix = glm::scale(tree_trunk_model_matrix, trunk_scale);
	tree_trunk_model_matrix = glm::translate(tree_trunk_model_matrix, glm::vec3(1.0f / 2, 1.0f / 2, 1.0f / 2));

	glm::vec3 crown_pos = position;
	crown_pos.y = trunk_scale.y;

	tree_crown_model_matrix = glm::mat4(1);
	tree_crown_model_matrix = glm::translate(tree_crown_model_matrix, crown_pos);

	

	tree_crown_model_matrix = glm::translate(tree_crown_model_matrix, glm::vec3(-crown_scale.x / 2, 0, -crown_scale.z / 2));
	tree_crown_model_matrix = glm::scale(tree_crown_model_matrix, crown_scale);
	tree_crown_model_matrix = glm::translate(tree_crown_model_matrix, glm::vec3(1.0f / 2, 1.0f / 2, 1.0f / 2));
}