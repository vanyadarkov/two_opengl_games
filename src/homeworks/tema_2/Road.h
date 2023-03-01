#pragma once

#include "homeworks/tema_2/utils.h"

using namespace std;
using namespace tema2_utils;

namespace tema2 
{
	class Road 
	{
	public:
		/**
		 * \brief Construieste un drum
		 * \param height inaltimea drumului (pozitia pe y al drumului)
		 * \param scale factorul de scalare a coordonatelor drumului
		 * \param color culoarea drumului
		 */
		Road(float height, glm::vec3 scale, glm::vec3 color);
		~Road();

		// banda interioara a traficului
		std::vector <VertexFormat> inner_traffic_lane;
		// banda exterioara a traficului
		std::vector <VertexFormat> outer_traffic_lane;
		// banda exteriora de spawn a copacilor
		std::vector <VertexFormat> outer_tree_lane;
		// banda interioara de spawn a copacilor
		std::vector <VertexFormat> inner_tree_lane;

		// curba din mijloc, exterior, interior a drumului
		std::vector<VertexFormat> middle_curve;
		std::vector<VertexFormat> outer_curve;
		std::vector<VertexFormat> inner_curve;
		// vertecsii si indicii drumului
		std::vector<VertexFormat> road_vertices;
		std::vector<unsigned int> road_indices;

		/**
		 * \brief Se creeaza meshul curbei din mijloc
		 * \param name numele meshului
		 * \return mesh creat
		 */
		Mesh* create_middle_curve(const std::string& name);
		/**
		 * \brief Se creeaza meshul curbei interioare a drumului
		 * \param name numele meshului
		 * \return mesh creat
		 */
		Mesh* create_inner_curve(const std::string& name);
		/**
		 * \brief Se creeaza meshul curbei exterioare a drumului
		 * \param name numele meshului
		 * \return mesh creat
		 */
		Mesh* create_outer_curve(const std::string& name);
		/**
		 * \brief Se creeaza meshul benzei interioare a traficului
		 * \param name numele meshului
		 * \return mesh creat
		 */
		Mesh* create_inner_lane_curve(const std::string& name);
		/**
		 * \brief Se creeaza meshul benzei exterioare a traficului
		 * \param name numele meshului
		 * \return mesh creat
		 */
		Mesh* create_outter_lane_curve(const std::string& name);

		/**
		 * \brief Initializeaza curba din mijloc (care sunt coordonatele punctelor din geogebra)
		 */
		void init_simple_curve();
		/**
		 * \brief Se discretizeaza curba din mijloc, si anumele se adauga puncte intermediare in segmentul descris de 2 puncte consecutive
		 */
		void compute_complex_curve();

		/**
		 * \brief Initializare curba de interior si exterior pe baza curbei din mijloc
		 * \param inner_curve_vertices lista unde se adauga vertecsii curbei interioare
		 * \param outer_curve_vertices lista unde se adauga vertecsii curbei exterioare
		 * \param width latimea (distanta de la mijloc la curbe)
		 * \param add_duplicates flag, care ne spune daca se adauga sau nu puncte duplicate (care pot aparea la capetele a 2 segmente consecutive)
		 */
		void init_inner_outer(vector<VertexFormat>& inner_curve_vertices, vector<VertexFormat>& outer_curve_vertices, float width, bool add_duplicates = true);

		/**
		 * \brief Initializeaza toate curbele drumului (dar mai intai discretizeaza curba din mijloc)
		 */
		void init_all_curves();

		/**
		 * \brief Construieste drumul propriu zis si intoarce meshul sau
		 * \param name numele meshului
		 * \return meshul drumului
		 */
		Mesh* build_road(const std::string& name);

		/**
		 * \brief Se face update in baza punctului primit ca parametru a punctelor extreme in care e cuprins drumul (traseul)
		 * \param point punctul de verificare
		 */
		void update_min_max_coords(glm::vec3 point);

		/**
		 * \brief Verificare daca punctul primit se afla sau nu in interiorul drumului
		 * \param point_to_check punctul care il verificam
		 * \return true in caz ca e pe drum, false invers
		 */
		bool check_if_on_road(glm::vec3 point_to_check);


		Mesh* road_mesh;
		float band_width;
		float max_x, max_z, min_x, min_z;
		glm::vec3 scale;
		float height;
	private:
		glm::vec3 color;
	};
}