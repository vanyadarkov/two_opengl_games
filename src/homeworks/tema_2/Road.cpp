#include "homeworks/tema_2/Road.h"

using namespace tema2;

Road::Road(float height, glm::vec3 scale, glm::vec3 color)
{
	this->scale = scale;
	this->height = height;
	this->band_width = scale.x / 5;
	this->color = color;
	min_x = min_z = INT32_MAX;
	max_x = max_z = INT32_MIN;
	road_mesh = build_road("road");
}

Road::~Road()
{
	
}

void Road::init_simple_curve()
{
	middle_curve.emplace_back(VertexFormat(glm::vec3(0, height, 4) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(0.4, height, 5.5) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(0.17, height, 6.89) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(-1.05, height, 8.33) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(-2.2, height, 9.5) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(-3.1, height, 11) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(-2.62, height, 12.41) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(-1.3, height, 13.1) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(0.5, height, 12.4) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(1.3, height, 10.6) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(2.57, height, 9.27) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(4.46, height, 9.09) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(5.62, height, 7.60) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(5.91, height, 6.37) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(5.00, height, 5.61) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(5.38, height, 4.02) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(6.93, height, 3.17) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(8.74, height, 3.83) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(9.25, height, 6.4) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(9.36, height, 8.36) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(8.45, height, 10.29) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(6, height, 12) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(4.46, height, 13.28) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(4.25, height, 15.16) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(6.26, height, 15.79) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(9.07, height, 15.15) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(11.07, height, 13.85) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(12.63, height, 11.96) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(13.89, height, 9.49) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(14.43, height, 7.03) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(14.1, height, 5.02) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(13.39, height, 3.27) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(12.43, height, 1.98) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(11.31, height, 0.39) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(10.18, height, -1.1) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(8.59, height, -1.93) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(7.01, height, -2.98) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(6, height, -4) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(4, height, -4) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(-0.33, height, -3.3) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(-3.3, height, -2.92) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(-6.5, height, -2.37) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(-9.22, height, -1.57) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(-9.42, height, 0.18) *scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(-8.31, height, 1.5) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(-5.63, height, 1.47) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(-3.29, height, 1.94) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(-1.95, height, 2.39) * scale, color));
	middle_curve.emplace_back(VertexFormat(glm::vec3(-0.56, height, 3.35) * scale, color));
}

void Road::update_min_max_coords(glm::vec3 point)
{
	if (point.x > max_x) {
		max_x = point.x;
	}
	if (point.x < min_x) {
		min_x = point.x;
	}
	if (point.z > max_z) {
		max_z = point.z;
	}
	if (point.z < min_z) {
		min_z = point.z;
	}
}


void Road::compute_complex_curve()
{
	std::vector<VertexFormat> newMiddle;
	
	for(int i = 0; i < middle_curve.size(); i++)
	{
		glm::vec3 p1 = middle_curve[i].position;
		glm::vec3 p2;
		if(i == middle_curve.size() - 1)
		{
			p2 = middle_curve[0].position;
		} else
		{
			p2 = middle_curve[i + 1].position;
		}
		newMiddle.emplace_back(VertexFormat(p1, color));
		std::vector<VertexFormat> between_points = discretize_the_segment(p1, p2, color);
		newMiddle.insert(newMiddle.end(), between_points.begin(), between_points.end());
	}
	newMiddle.emplace_back(middle_curve[0]);

	middle_curve = newMiddle;
}

void Road::init_inner_outer(vector<VertexFormat>& inner_curve_vertices, vector<VertexFormat>& outer_curve_vertices, float width, bool add_duplicates)
{
	glm::vec3 previous_right_inner = glm::vec3(0);
	glm::vec3 previous_right_outer = glm::vec3(0);
	glm::vec3 previous_left_inner = glm::vec3(0);
	glm::vec3 previous_left_outer = glm::vec3(0);
	glm::vec3 up = { 0, 1, 0 };
	for (unsigned long long i = 0; i < middle_curve.size() - 1; i++)
	{
		glm::vec3 d;
		glm::vec3 p1 = middle_curve[i].position;
		glm::vec3 p2 = middle_curve[i + 1].position;
		d = p2 - p1;
		glm::vec3 P = glm::normalize(glm::cross(d, up));
		glm::vec3 leftInnerPoint = p1 - width * P;
		leftInnerPoint.y = height;
		glm::vec3 leftOuterPoint = p1 + width * P;
		leftOuterPoint.y = height;
		glm::vec3 rightInnerPoint = p2 - width * P;
		rightInnerPoint.y = height;
		glm::vec3 rightOuterPoint = p2 + width * P;
		rightOuterPoint.y = height;
		update_min_max_coords(leftInnerPoint);
		update_min_max_coords(leftOuterPoint);
		update_min_max_coords(rightInnerPoint);
		update_min_max_coords(rightOuterPoint);
		if (i > 0)
		{
			glm::vec3 intersection_inner = intersection_point_of_two_lines(previous_right_inner, previous_left_inner, leftInnerPoint, rightInnerPoint);
			if (intersection_inner == glm::vec3(0.f, -100.f, 0.f))
			{
				inner_curve_vertices.emplace_back(VertexFormat(previous_right_inner, color));
				if((previous_right_inner != leftInnerPoint && !add_duplicates) || add_duplicates)
					inner_curve_vertices.emplace_back(VertexFormat(leftInnerPoint, color));
			}
			else
			{
				inner_curve_vertices.emplace_back(VertexFormat(intersection_inner, color));
				if(add_duplicates) inner_curve_vertices.emplace_back(VertexFormat(intersection_inner, color));
			}
			glm::vec3 intersection_outer = intersection_point_of_two_lines(previous_right_outer, previous_left_outer, leftOuterPoint, rightOuterPoint);
			if (intersection_outer == glm::vec3(0.f, -100.f, 0.f))
			{
				outer_curve_vertices.emplace_back(VertexFormat(previous_right_outer, color));
				if ((previous_right_outer != leftOuterPoint && !add_duplicates) || add_duplicates)
					outer_curve_vertices.emplace_back(VertexFormat(leftOuterPoint, color));
			}
			else
			{
				outer_curve_vertices.emplace_back(VertexFormat(intersection_outer, color));
				if(add_duplicates) outer_curve_vertices.emplace_back(VertexFormat(intersection_outer, color));
			}
		}
		previous_right_inner = rightInnerPoint;
		previous_right_outer = rightOuterPoint;
		previous_left_inner = leftInnerPoint;
		previous_left_outer = leftOuterPoint;
	}
	glm::vec3 d;
	glm::vec3 p1 = middle_curve[0].position;
	glm::vec3 p2 = middle_curve[1].position;
	d = p2 - p1;
	glm::vec3 P = glm::normalize(glm::cross(d, up));
	glm::vec3 leftInnerPoint = p1 - width * P;
	leftInnerPoint.y = height;
	glm::vec3 leftOuterPoint = p1 + width * P;
	leftOuterPoint.y = height;
	glm::vec3 rightInnerPoint = p2 - width * P;
	rightInnerPoint.y = height;
	glm::vec3 rightOuterPoint = p2 + width * P;
	rightOuterPoint.y = height;
	glm::vec3 intersection_inner = intersection_point_of_two_lines(previous_left_inner, previous_right_inner, leftInnerPoint, rightInnerPoint);
	if (intersection_inner == glm::vec3(0, -100, 0))
	{
		inner_curve_vertices.emplace_back(VertexFormat(leftInnerPoint, color));
		if(add_duplicates) inner_curve_vertices.emplace(inner_curve_vertices.begin(), VertexFormat(leftInnerPoint, color));
	}
	else
	{
		inner_curve_vertices.emplace_back(VertexFormat(intersection_inner, color));
		if(add_duplicates) inner_curve_vertices.emplace(inner_curve_vertices.begin(), VertexFormat(intersection_inner, color));
	}
	glm::vec3 intersection_outer = intersection_point_of_two_lines(previous_left_outer, previous_right_outer, leftOuterPoint, rightOuterPoint);
	if (intersection_outer == glm::vec3(0, -100, 0))
	{
		outer_curve_vertices.emplace_back(VertexFormat(leftOuterPoint, color));
		if(add_duplicates) outer_curve_vertices.emplace(outer_curve_vertices.begin(), VertexFormat(leftOuterPoint, color));
	}
	else
	{
		outer_curve_vertices.emplace_back(VertexFormat(intersection_outer, color));
		if(add_duplicates)outer_curve_vertices.emplace(outer_curve_vertices.begin(), VertexFormat(intersection_outer, color));
	}
}

void Road::init_all_curves()
{
	init_simple_curve();
	
	compute_complex_curve();
	init_inner_outer(inner_traffic_lane, outer_traffic_lane, band_width / 2, false);
	init_inner_outer(inner_curve, outer_curve, band_width);
	init_inner_outer(inner_tree_lane, outer_tree_lane, band_width * 1.7f, false);
}

Mesh* Road::create_middle_curve(const std::string& name)
{
	std::vector<unsigned int> indices;
	Mesh* middleMesh = new Mesh(name);
	middleMesh->SetDrawMode(GL_LINE_STRIP);

	int i = 0;
	for (auto x : middle_curve)
	{
		indices.push_back(i++);
	}

	middleMesh->InitFromData(middle_curve, indices);
	return middleMesh;
}

Mesh* Road::create_inner_curve(const std::string& name)
{
	std::vector<unsigned int> indices;
	Mesh* innerMesh = new Mesh(name);
	innerMesh->SetDrawMode(GL_LINE_STRIP);

	int i = 0;
	for (auto x : inner_curve)
	{
		indices.push_back(i++);
	}

	innerMesh->InitFromData(inner_curve, indices);
	return innerMesh;
}

Mesh* Road::create_outer_curve(const std::string& name)
{
	std::vector<unsigned int> indices;
	Mesh* outerMesh = new Mesh(name);
	outerMesh->SetDrawMode(GL_LINE_STRIP);

	int i = 0;
	for (auto x : outer_curve)
	{
		indices.push_back(i++);
	}

	outerMesh->InitFromData(outer_curve, indices);
	return outerMesh;
}

Mesh* Road::create_inner_lane_curve(const std::string& name)
{
	std::vector<unsigned int> indices;
	Mesh* inner_lane_mesh = new Mesh(name);
	inner_lane_mesh->SetDrawMode(GL_LINE_LOOP);

	int i = 0;
	for (auto x : inner_traffic_lane)
	{
		indices.push_back(i++);
	}

	inner_lane_mesh->InitFromData(inner_traffic_lane, indices);
	return inner_lane_mesh;
}

Mesh* Road::create_outter_lane_curve(const std::string& name)
{
	std::vector<unsigned int> indices;
	Mesh* outter_lane_mesh = new Mesh(name);
	outter_lane_mesh->SetDrawMode(GL_LINE_LOOP);

	int i = 0;
	for (auto x : outer_traffic_lane)
	{
		indices.push_back(i++);
	}

	outter_lane_mesh->InitFromData(outer_traffic_lane, indices);
	return outter_lane_mesh;
}


Mesh* Road::build_road(const std::string& name)
{
	init_all_curves();
	Mesh* roadMesh = new Mesh(name);
	int nr_vertices = 0;
	bool add_current_left_inner = false;
	bool add_current_left_outer = false;
	for (int i = 0; i < middle_curve.size() - 1; i++)
	{
		road_vertices.push_back(outer_curve[2 * i]);		// left			0
		int left_outer_index = nr_vertices++;
		road_vertices.push_back(outer_curve[2 * i + 1]);	// right		1
		int right_outer_index = nr_vertices++;
		road_vertices.push_back(inner_curve[2 * i]);		// left			2
		int left_inner_index = nr_vertices++;
		road_vertices.push_back(inner_curve[2 * i + 1]);	// right		3
		int right_inner_index = nr_vertices++;
		if (add_current_left_inner) road_indices.push_back(left_inner_index);
		if (add_current_left_outer) road_indices.push_back(left_outer_index);
		add_current_left_inner = false;
		add_current_left_outer = false;

		road_indices.push_back(left_outer_index);
		road_indices.push_back(left_inner_index);
		road_indices.push_back(right_outer_index);

		road_indices.push_back(right_outer_index);
		road_indices.push_back(left_inner_index);
		road_indices.push_back(right_inner_index);

		if (i < middle_curve.size() - 2)
		{
			glm::vec3 current_right_inner = inner_curve[2 * i + 1].position;
			glm::vec3 next_left_inner = inner_curve[2 * (i + 1)].position;
			if (!check_if_equal_points(current_right_inner, next_left_inner))
			{
				road_indices.push_back(right_outer_index);
				road_indices.push_back(right_inner_index);
				add_current_left_inner = true;
			}
			glm::vec3 current_right_outer = outer_curve[2 * i + 1].position;
			glm::vec3 next_left_outer = outer_curve[2 * (i + 1)].position;
			if (!check_if_equal_points(current_right_outer, next_left_outer))
			{
				road_indices.push_back(right_outer_index);
				road_indices.push_back(right_inner_index);
				add_current_left_outer = true;
			}
		}
	}
	//roadMesh->SetDrawMode(GL_LINE_LOOP);
	roadMesh->InitFromData(road_vertices, road_indices);
	return roadMesh;
}


bool Road::check_if_on_road(glm::vec3 point_to_check)
{
	for(int i = 0; i < road_indices.size(); i += 3)
	{
		glm::vec3 a = road_vertices[road_indices[i]].position;
		
		glm::vec3 b = road_vertices[road_indices[i + 1]].position;
		
		glm::vec3 c = road_vertices[road_indices[i + 2]].position;
		
		if(point_in_triangle(point_to_check, a, b, c) == true)
		{
			return true;
		}
	}
	return false;
}