#include "homeworks/tema_2/utils.h"

bool same_side(glm::vec3 p1, glm::vec3 p2, glm::vec3 a, glm::vec3 b)
{
	glm::vec3 cp1 = glm::cross(b - a, p1 - a);
	glm::vec3 cp2 = glm::cross(b - a, p2 - a);
	if (glm::dot(cp1, cp2) >= 0) return true;
	return false;
}

bool tema2_utils::point_in_triangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	return same_side(p, a, b, c) && same_side(p, b, a, c) && same_side(p, c, a, b);
}

std::vector<VertexFormat> tema2_utils::discretize_the_segment(glm::vec3 p1, glm::vec3 p2, glm::vec3 color)
{
	std::vector<VertexFormat> res;
	// Numarul de puncte adaugate egal cu lungimea segmentului
	int pointNum = glm::length(p2 - p1);
	float diff_x = p2.x - p1.x;
	float diff_z = p2.z - p1.z;
	float interval_x = diff_x / (pointNum + 1);
	float interval_z = diff_z / (pointNum + 1);
	for (int j = 1; j <= pointNum; j++)
	{
		glm::vec3 newP = { p1.x + interval_x * j, p1.y, p1.z + interval_z * j };
		res.emplace_back(VertexFormat(newP, color));
	}
	return res;
}

bool tema2_utils::check_if_equal_points(glm::vec3 p1, glm::vec3 p2, float eps)
{
	return (fabs(p1.x - p2.x) < eps) && (fabs(p1.y - p2.y) < eps) && (fabs(p1.z - p2.z) < eps);
}

glm::vec3 tema2_utils::intersection_point_of_two_lines(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 D)
{
	if (check_if_equal_points(B, C)) return B;
	double a1 = B.z - A.z;
	double b1 = A.x - B.x;
	double c1 = a1 * (A.x) + b1 * (A.z);

	double a2 = D.z - C.z;
	double b2 = C.x - D.x;
	double c2 = a2 * (C.x) + b2 * (C.z);

	double determinant = a1 * b2 - a2 * b1;
	if (fabs(determinant) <= DBL_EPSILON)
	{
		return { 0.f, -100.f, 0.f };
	}
	float x = (b2 * c1 - b1 * c2) / determinant;
	float z = (a1 * c2 - a2 * c1) / determinant;
	if (min(A.x, B.x) <= x && x <= max(A.x, B.x))
	{
		if (min(A.z, B.z) <= z && z <= max(A.z, B.z))
		{
			if (min(C.x, D.x) <= x && x <= max(C.x, D.x))
			{
				if (min(C.z, D.z) <= z && z <= max(C.z, D.z))
				{
					return { x, A.y, z };
				}
			}
		}
	}
	return { 0.f, -100.f, 0.f };
}

int tema2_utils::get_next_or_prev_index_from_vector(int current_index, vector<VertexFormat> vec, bool next)
{
	if(next)
	{
		int next_index = current_index + 1;
		if (next_index == vec.size()) next_index = 0;
		return next_index;
	} else
	{
		int prev_index = current_index - 1;
		if (prev_index == -1) prev_index = vec.size() - 1;
		return prev_index;
	}
}

Mesh* tema2_utils::create_simple_cube(std::string name, glm::vec3 color)
{
	// din lab 6
	float half = 1.0f / 2;
	vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(-half, -half,  half), color),
		VertexFormat(glm::vec3(half, -half,  half), color),
		VertexFormat(glm::vec3(-half,  half,  half),color),
		VertexFormat(glm::vec3(half,  half,  half), color),
		VertexFormat(glm::vec3(-half, -half, -half), color),
		VertexFormat(glm::vec3(half, -half, -half), color),
		VertexFormat(glm::vec3(-half,  half, -half),color),
		VertexFormat(glm::vec3(half,  half, -half), color)
	};

	vector<unsigned int> indices =
	{
		0, 1, 2,        1, 3, 2,
		2, 3, 7,        2, 7, 6,
		1, 7, 3,        1, 5, 7,
		6, 7, 4,        7, 5, 4,
		0, 4, 1,        1, 4, 5,
		2, 6, 4,        0, 2, 4,
	};

	Mesh* cube_mesh = new Mesh(name);
	cube_mesh->InitFromData(vertices, indices);
	return cube_mesh;
}