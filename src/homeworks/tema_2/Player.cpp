#include "homeworks/tema_2/Player.h"


using namespace tema2;

Player::Player(std::string name, glm::vec3 scale, glm::vec3 color, Road* road)
{
	this->color = color;
	this->scale = scale;
	player_mesh = tema2_utils::create_simple_cube("player_car_mesh", color);
	max_speed = scale.x * 7.5f;
	current_speed = 0;
	max_wheels_rotation_angle = 2;
	current_wheels_rotation_angle = 0;
	this->road = road;
	this->direction_angle = 0;
}

Player::~Player()
{
	
}

vector<glm::vec3> Player::get_player_car_corners()
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
	vector<glm::vec3> corners = { p1 + position, p2 + position, p3 + position, p4 + position};
	return corners;
}

void Player::spawn_player(glm::vec3 position, glm::vec3 forward)
{
	this->position = position;
	this->forward = glm::normalize(forward);
	update_model_matrix();
}

void Player::update_model_matrix()
{
	model_matrix = glm::mat4(1);
	model_matrix = glm::translate(model_matrix, position);

	float cos_value = glm::dot(forward, glm::vec3(1, 0, 0));
	direction_angle = acos(cos_value);

	glm::vec3 cross_product = glm::cross(forward, glm::vec3(1, 0, 0));
	if (cross_product.x  >= 0 && cross_product.y >= 0 && cross_product.z >= 0)
	{
		direction_angle = 2 * M_PI - direction_angle;
	}
	model_matrix = glm::rotate(model_matrix, direction_angle, glm::vec3(0, 1, 0));


	model_matrix = glm::translate(model_matrix, glm::vec3(-scale.x / 2, 0, -scale.z / 2));
	model_matrix = glm::scale(model_matrix, scale);
	model_matrix = glm::translate(model_matrix, glm::vec3(1.0f / 2, 1.0f / 2, 1.0f / 2));
}


void Player::move(float deltaTime)
{
	move_forward(deltaTime);
	move_right(deltaTime);
	update_model_matrix();
}

void Player::stop()
{
	current_speed = 0;
	current_wheels_rotation_angle = 0;
}

void Player::move_forward(float deltaTime)
{
	if(!accelerating)
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
	glm::vec3 new_pos = position + glm::normalize(forward) * (deltaTime * current_speed);
	bool on_road = road->check_if_on_road(new_pos);
	if (on_road)
	{
		position = new_pos;
	} else
	{
		stop();
	}
	
	accelerating = false;
}

void Player::accelerate(float deltaTime)
{
	
	accelerating = true;
	if(sgn(deltaTime) != sgn(current_speed))
	{
		current_speed += 4 * deltaTime;
	} else
	{
		current_speed += 2 * deltaTime;
	}
	
	if (abs(current_speed) > max_speed) current_speed = sgn(current_speed) * max_speed;
}

void Player::move_right(float deltaTime)
{
	if(!turning_right)
	{
		float new_rotation_angle = current_wheels_rotation_angle + 2 * deltaTime * -sgn(current_wheels_rotation_angle);
		if (sgn(new_rotation_angle) != sgn(current_wheels_rotation_angle))
		{
			current_wheels_rotation_angle = 0;
		}
		else
		{
			current_wheels_rotation_angle = new_rotation_angle;
		}
	}
	turning_right = false;
	glm::vec4 newVec = glm::rotate(glm::mat4(1.f), RADIANS(current_wheels_rotation_angle), glm::vec3(0, 1, 0)) * glm::vec4(forward, 0);
	forward = glm::normalize(glm::vec3(newVec));
}

void Player::turn_right(float deltaTime)
{
	if (current_speed == 0) return;
	turning_right = true;
	if(sgn(deltaTime) != sgn(current_wheels_rotation_angle))
	{
		current_wheels_rotation_angle += 3 * deltaTime;
	} else
	{
		current_wheels_rotation_angle += deltaTime * 1.5f;
	}
	if (abs(current_wheels_rotation_angle) > max_wheels_rotation_angle) current_wheels_rotation_angle = sgn(current_wheels_rotation_angle) * max_wheels_rotation_angle;
}
