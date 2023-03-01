#include "homeworks/tema_2/Bot.h"

using namespace tema2;
using namespace std;

Bot::Bot(std::string name, glm::vec3 scale, glm::vec3 color)
{
	this->color = color;
	this->scale = scale;
	bot_mesh = tema2_utils::create_simple_cube(name, color);
	max_speed = scale.x * 7.5f;
	current_speed = 0;
	this->direction_angle = 0;
}

Bot::~Bot()
{
}

vector<glm::vec3> Bot::get_bot_car_corners()
{
	float x_cord = scale.x / 2;
	float z_cord = scale.z / 2;
	float cos_value = cos(-direction_angle);
	float sin_value = sin(-direction_angle);

	glm::vec3 p1 = { x_cord * cos_value - z_cord * sin_value, position.y,x_cord * sin_value + z_cord * cos_value };
	x_cord *= -1;
	glm::vec3 p2 = { x_cord * cos_value - z_cord * sin_value, position.y, x_cord * sin_value + z_cord * cos_value };
	z_cord *= -1;
	glm::vec3 p3 = { x_cord * cos_value - z_cord * sin_value, position.y, x_cord * sin_value + z_cord * cos_value };
	x_cord *= -1;
	glm::vec3 p4 = { x_cord * cos_value - z_cord * sin_value, position.y, x_cord * sin_value + z_cord * cos_value };
	vector<glm::vec3> corners = { p1 + position, p2 + position, p3 + position, p4 + position };
	return corners;
}

void Bot::spawn_bot(glm::vec3 position, glm::vec3 forward)
{
	this->position = position;
	this->forward = glm::normalize(forward);
	update_model_matrix();
}

void Bot::update_model_matrix()
{
	model_matrix = glm::mat4(1);
	model_matrix = glm::translate(model_matrix, position);

	float cos_value = glm::dot(forward, glm::vec3(1, 0, 0));
	direction_angle = acos(cos_value);

	glm::vec3 cross_product = glm::cross(forward, glm::vec3(1, 0, 0));
	if (cross_product.x >= 0 && cross_product.y >= 0 && cross_product.z >= 0)
	{
		direction_angle = 2 * M_PI - direction_angle;
	}
	model_matrix = glm::rotate(model_matrix, direction_angle, glm::vec3(0, 1, 0));


	model_matrix = glm::translate(model_matrix, glm::vec3(-scale.x / 2, 0, -scale.z / 2));
	model_matrix = glm::scale(model_matrix, scale);
	model_matrix = glm::translate(model_matrix, glm::vec3(1.0f / 2, 1.0f / 2, 1.0f / 2));
}

void Bot::stop()
{
	this->current_speed = 0;
}


void Bot::move(float deltaTime)
{
	move_forward(deltaTime);
	update_model_matrix();
}

void Bot::move_forward(float deltaTime)
{
	if (!accelerating)
	{
		float new_speed = current_speed + 2 * deltaTime * -sgn(current_speed);
		if (sgn(new_speed) != sgn(current_speed))
		{
			current_speed = 0;
		}
		else
		{
			current_speed = new_speed;
		}
	}
	glm::vec3 last_pos = position;
	position += glm::normalize(forward) * (deltaTime * current_speed);
	accelerating = false;
}

void Bot::accelerate(float deltaTime)
{
	accelerating = true;
	if (sgn(deltaTime) != sgn(current_speed))
	{
		current_speed += 4 * deltaTime;
	}
	else
	{
		current_speed += 2 * deltaTime;
	}

	if (abs(current_speed) > max_speed) current_speed = sgn(current_speed) * max_speed;
}