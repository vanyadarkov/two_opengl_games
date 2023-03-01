#include "homeworks/tema_2/Terrain.h"

using namespace tema2;

Terrain::Terrain(float min_x, float max_x, float min_z, float max_z, float height, glm::vec3 color)
{
	this->min_x = min_x - TERRAIN_OFFSET;
	this->max_x = max_x + TERRAIN_OFFSET;
	this->min_z = min_z - TERRAIN_OFFSET;
	this->max_z = max_z + TERRAIN_OFFSET;
	this->height = height;
	this->color = color;
	modelMatrix = glm::mat4(1);
	create_terrain();
}

void Terrain::discretize_terrain()
{
	// se discretizeaza latura din stanga
	glm::vec3 p1 = glm::vec3(min_x, height, min_z);
	glm::vec3 p2 = glm::vec3(min_x, height, max_z);
	vector<VertexFormat> left_side;
	left_side.emplace_back(VertexFormat(p1, color));
	vector<VertexFormat> left_side_res = tema2_utils::discretize_the_segment(p1, p2, color);
	left_side.insert(left_side.end(), left_side_res.begin(), left_side_res.end());
	left_side.emplace_back(VertexFormat(p2, color));
	// se discretizeaza latura inferioara
	p1 = glm::vec3(min_x, height, min_z);
	p2 = glm::vec3(max_x, height, min_z);
	vector<VertexFormat> bottom_side;
	bottom_side.emplace_back(VertexFormat(p1, color));
	vector<VertexFormat> bottom_side_res = tema2_utils::discretize_the_segment(p1, p2, color);
	bottom_side.insert(bottom_side.end(), bottom_side_res.begin(), bottom_side_res.end());
	bottom_side.emplace_back(VertexFormat(p2, color));

	// calculare si adaugare vertecsi si indecsi
	for(int i = 1; i < left_side.size(); i++)
	{
		for(int j = 1; j < bottom_side.size(); j++)
		{
			glm::vec3 lb = glm::vec3(bottom_side[j - 1].position.x, height, left_side[i - 1].position.z);
			glm::vec3 rb = glm::vec3(bottom_side[j].position.x, height, left_side[i - 1].position.z);
			glm::vec3 lu = glm::vec3(bottom_side[j - 1].position.x, height, left_side[i].position.z);
			glm::vec3 ru = glm::vec3(bottom_side[j].position.x, height, left_side[i].position.z);
			terrain_vertices.emplace_back(VertexFormat(lb, color));
			terrain_vertices.emplace_back(VertexFormat(rb, color));
			terrain_vertices.emplace_back(VertexFormat(lu, color));
			terrain_vertices.emplace_back(VertexFormat(ru, color));
			terrain_indices.push_back(terrain_vertices.size() - 1);
			terrain_indices.push_back(terrain_vertices.size() - 2);
			terrain_indices.push_back(terrain_vertices.size() - 4);
			terrain_indices.push_back(terrain_vertices.size() - 1);
			terrain_indices.push_back(terrain_vertices.size() - 4);
			terrain_indices.push_back(terrain_vertices.size() - 3);
		}
	}
}

void Terrain::create_terrain()
{
	discretize_terrain();
	
	terrain_mesh = new Mesh("terrain_mesh");

	terrain_mesh->InitFromData(terrain_vertices, terrain_indices);
}


