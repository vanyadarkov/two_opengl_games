#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <tuple>

#include "utils/gl_utils.h"
#include "utils/glm_utils.h"
#include "core/gpu/mesh.h"
#include "utils/math_utils.h"


// 0.0001f

using namespace std;

namespace tema2_utils
{
	/**
	 * \brief Verificarea daca punctul P se afla in triunghiul descris de ABC
	 *
	 * \note https://blackpawn.com/texts/pointinpoly/
	 *
	 * \param p punctul p
	 * \param a punctul a al triunghiului
	 * \param b punctul b al triunghiului
	 * \param c punctul c al triunghiului
	 * \return true -> in interior, false caz contrar
	 */
	bool point_in_triangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c);

	/**
	 * \brief Discretizeaza un segment descris de 2 puncte si creeaza vertecsi de culoarea color intre ele
	 * \param p1 punctul 1
	 * \param p2 punctul 2
	 * \param color culoare
	 * \return lista cu vertecsii obtinuti in urma discretizarii
	 */
	std::vector<VertexFormat> discretize_the_segment(glm::vec3 p1, glm::vec3 p2, glm::vec3 color);

	/**
	 * \brief Verifica daca doua puncte sunt egale
	 * \param p1 p1
	 * \param p2 p2
	 * \param eps eroare (by default 0.0001f)
	 * \return true -> egale, false caz contrar
	 */
	bool check_if_equal_points(glm::vec3 p1, glm::vec3 p2, float eps = 0.0001f);

	/**
	 * \brief Obtine punctul de intersectie intre 2 segmente descrie de cate 2 puncte
	 * \param A Punctul A al primului segment
	 * \param B Punctul B al primului segment
	 * \param C Punctul C al celui de al doilea segment
	 * \param D Punctul D al celui de al doilea segment
	 * \return { 0.f, -100.f, 0.f } -> nu exista intersectie, in caz contrar punctul de intersectie
	 */
	glm::vec3 intersection_point_of_two_lines(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 D);

	/**
	 * \brief Obtine indexul urmatoru intr-un vector
	 * \param current_index indexul curent
	 * \param vec vectorul din care dorim sa obtinem indexul
	 * \param next daca e true -> urmatorul va fi curent + 1, daca false -> urmatorul va fi curent - 1
	 * \return indexul urmator
	 */
	int get_next_or_prev_index_from_vector(int current_index, vector<VertexFormat> vec, bool next = true);

	/**
	 * \brief Creeaza un mesh ce reprezinta un cub (cu latura 1, coltul stanga jos in origine)
	 * \param name numele meshului pentru cub
	 * \param color culoarea meshului (cubului)
	 * \return Mesh pentru cub
	 */
	Mesh* create_simple_cube(std::string name, glm::vec3 color);

	/**
	 * \brief Obtine semnul unui numar
	 * \tparam T Tipul de date (int, float, double etc)
	 * \param val valoarea numerica
	 * \return 1 sau -1;
	 */
	template <typename T> int sgn(T val) {
		return (T(0) < val) - (val < T(0));
	}

	/**
	 * \brief Genereaza random un numar intr-un interval
	 * \tparam T Tipul valorii generate
	 * \param low limita de jos a intervalului
	 * \param high limita de sus a intervalului
	 * \return valoare de tip T generata random in intervalul specificat
	 */
	template <typename T> T random_in_a_range(T low, T high)
	{
		return low + static_cast <T> (rand()) / (RAND_MAX / (high - low));
	}
}