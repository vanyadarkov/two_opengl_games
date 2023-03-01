#pragma once

#include "homeworks/tema_2/utils.h"

using namespace std;
constexpr float TERRAIN_OFFSET = 50;

namespace tema2
{
	class Terrain
	{
	public:
		/**
		 * \brief Creare teren
		 * \param min_x coordonata x minima (care deobicei o extragem din drumul creat)
		 * \param max_x coordonata x maxima (care deobicei o extragem din drumul creat)
		 * \param min_z coordonata z minima (care deobicei o extragem din drumul creat)
		 * \param max_z coordonata z maxima (care deobicei o extragem din drumul creat)
		 * \param height inaltimea la care va fi situat terenul
		 * \param color culoarea terenului
		 */
		Terrain(float min_x, float max_x, float min_z, float max_z, float height, glm::vec3 color);
		~Terrain();
		/**
		 * \brief Crearea mesh pentru teren
		 */
		void create_terrain();

		/**
		 * \brief Discretizarea (adaugarea mai multor triunghiuri) pentru mesh
		 */
		void discretize_terrain();


		float min_x, min_z;
		float max_x, max_z;
		Mesh* terrain_mesh;
		glm::mat4 modelMatrix;
		glm::vec3 color;
		float height;
		std::vector<VertexFormat> terrain_vertices;
		std::vector<unsigned int> terrain_indices;
	private:

	};
}