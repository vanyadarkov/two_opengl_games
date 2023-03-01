#include "mainEvent.h"

using namespace tema1;
using namespace std;

Duck::Duck(int resolutionX, int resolutionY)
{
	duckState = DUCK_STATE_ACTIVE;
	tailX = 0;
	tailY = 0;
	defaultTailScale = resolutionX / 50.0f;
	tailScaleX = defaultTailScale;
	tailScaleY = tailScaleX;
	bodyScaleX = tailScaleX / 1.3f;
	bodyScaleY = tailScaleY * 2.25f;
	wingScaleX = tailScaleX * 1.25f;
	wingScaleY = tailScaleY * 3.5;
	wingAngle = defaultWingAngle * 2;
	wingState = WING_STATE_OUT;
	headScaleX = tailScaleX;
	headScaleY = tailScaleY;
	beakScaleX = tailScaleX / 1.3f;
	beakScaleY = tailScaleY / 1.3f;

	direction = 90;

	scoreForDuck = defaultScoreForDuck;
	timeAlive = 0;
	directionChangerCount = 0;
	speed = resolutionX / 2.8f;

	drawHitbox = false;

	duckBody = object2D::CreateCircle("duckBody", left_bottom_corner, 1, 100, color_gray, true);
	duckTail = object2D::CreateTriangle("duckTail", left_bottom_corner, 1, color_black, true, RIGHT_ANGLE_TRIANGLE);
	duckHead = object2D::CreateCircle("duckHead", left_bottom_corner, 1, 100, color_green, true);
	duckBeak = object2D::CreateTriangle("duckBeak", left_bottom_corner, 1, color_orange, true, EQUILATERAL_TRIANGLE);
	duckLeftWing = object2D::CreateTriangle("duckLeftWing", left_bottom_corner, 1, color_gray, true, RIGHT_ANGLE_TRIANGLE);
	duckRightWing = object2D::CreateTriangle("duckLeftWing", left_bottom_corner, 1, color_gray, true, RIGHT_ANGLE_TRIANGLE);
	hitbox = object2D::CreateSquare("square_test", left_bottom_corner, 1, color_black, false);

	updateDuckProperties();
}

Duck::~Duck()
{
}


glm::vec2 Duck::getSize() {
	float sizeOx = bodyScaleX * 2 + wingScaleX * 2;
	float sizeOy = tailScaleY + bodyScaleY * 2 + headScaleY * 2 + beakScaleY;
	return glm::vec2(sizeOx, sizeOy);
}

std::vector<Mesh*> Duck::getAllMeshes()
{
	std::vector<Mesh*> allMeshes;
	allMeshes.push_back(duckBody);
	allMeshes.push_back(duckTail);
	allMeshes.push_back(duckHead);
	allMeshes.push_back(duckBeak);
	allMeshes.push_back(duckLeftWing);
	allMeshes.push_back(duckRightWing);

	return allMeshes;
}

void Duck::updateDuckProperties() {
	tailScaleY = tailScaleX;
	bodyScaleX = tailScaleX / 1.3f;
	bodyScaleY = tailScaleY * 2.25f;
	wingScaleX = tailScaleX * 1.25f;
	wingScaleY = tailScaleY * 3.5;
	headScaleX = tailScaleX;
	headScaleY = tailScaleY;
	beakScaleX = tailScaleX / 1.3f;
	beakScaleY = tailScaleY / 1.3f;

	bodyX = tailX;
	bodyY = tailY + tailScaleY; // body start (bottom margin)
	leftWingX = bodyX - bodyScaleX;
	leftWingY = bodyY + 3.0f / 2 * bodyScaleY;
	rightWingX = bodyX + bodyScaleX;
	rightWingY = leftWingY;
	headX = bodyX;
	headY = bodyY + 2 * bodyScaleY;
	beakX = bodyX;
	beakY = headY + 2 * headScaleY;
}

glm::vec2 Duck::getBeakPeakRealCoordinates() {
	float realDirectionRadians = RADIANS(direction - 90);
	return glm::vec2((beakX - tailX) * cos(realDirectionRadians) - (beakY + beakScaleY - tailY) * sin(realDirectionRadians) + tailX,
						(beakX - tailX) * sin(realDirectionRadians) + (beakY + beakScaleY - tailY) * cos(realDirectionRadians) + tailY);
}

std::unordered_map<Mesh*, glm::mat3> Duck::drawDuck()
{
	float realDirectionRadians = RADIANS(direction - 90);
	float sizeOx = getSize().x;
	float sizeOy = getSize().y;

	std::unordered_map<Mesh*, glm::mat3> meshesToRender;
	glm::mat3 modelMatrix;
	// BODY
	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(bodyX, bodyY);
	// Rotate by direction
	modelMatrix *= transform2D::Translate(0, -tailScaleY);
	modelMatrix *= transform2D::Rotate(realDirectionRadians);
	modelMatrix *= transform2D::Translate(0, tailScaleY);
	//
	modelMatrix *= transform2D::Translate(0, bodyScaleY); // bottom margin is in center of xOy (before this, corner was the center of circle)
	modelMatrix *= transform2D::Scale(bodyScaleX, bodyScaleY);
	meshesToRender.insert(std::make_pair(duckBody, modelMatrix));

	// TAIL
	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(tailX, tailY);
	// Rotate by direction
	modelMatrix *= transform2D::Rotate(realDirectionRadians);
	//
	modelMatrix *= transform2D::Rotate(RADIANS(90));
	modelMatrix *= transform2D::Scale(tailScaleX, tailScaleY);
	meshesToRender.insert(std::make_pair(duckTail, modelMatrix));

	// HEAD
	
	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(headX, headY);
	// Rotate by direction
	modelMatrix *= transform2D::Translate(0, -1 * (tailScaleY + 2 * bodyScaleY));
	modelMatrix *= transform2D::Rotate(realDirectionRadians);
	modelMatrix *= transform2D::Translate(0, tailScaleY + 2 * bodyScaleY);
	//
	modelMatrix *= transform2D::Translate(0, headScaleY);
	modelMatrix *= transform2D::Scale(headScaleX, headScaleY);
	meshesToRender.insert(std::make_pair(duckHead, modelMatrix));

	// BEAK
	
	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(beakX, beakY);
	// Rotate by direction
	modelMatrix *= transform2D::Translate(0, -1 * (tailScaleY + 2 * bodyScaleY + 2 * headScaleY));
	modelMatrix *= transform2D::Rotate(realDirectionRadians);
	modelMatrix *= transform2D::Translate(0, tailScaleY + 2 * bodyScaleY + 2 * headScaleY);
	// 
	modelMatrix *= transform2D::Translate(-(beakScaleX / 2), 0);
	modelMatrix *= transform2D::Scale(beakScaleX, beakScaleY);
	meshesToRender.insert(std::make_pair(duckBeak, modelMatrix));

	// LEFT WING
	
	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(leftWingX, leftWingY);
	// Rotate by direction
	modelMatrix *= transform2D::Translate(bodyScaleX, -1 * (tailScaleY + (3.0f / 2 * bodyScaleY)));
	modelMatrix *= transform2D::Rotate(realDirectionRadians);
	modelMatrix *= transform2D::Translate(-bodyScaleX, tailScaleY + (3.0f / 2 * bodyScaleY));
	//
	modelMatrix *= transform2D::Rotate(-RADIANS(wingAngle));
	modelMatrix *= transform2D::Scale(wingScaleX, wingScaleY);
	modelMatrix *= transform2D::Rotate(RADIANS(180));
	meshesToRender.insert(std::make_pair(duckLeftWing, modelMatrix));

	// RIGHT WING
	
	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(rightWingX, rightWingY);
	//put direction
	modelMatrix *= transform2D::Translate(-bodyScaleX, -1 * (tailScaleY + 3.0f / 2 * bodyScaleY));
	modelMatrix *= transform2D::Rotate(realDirectionRadians);
	modelMatrix *= transform2D::Translate(bodyScaleX, tailScaleY + 3.0f / 2 * bodyScaleY);
	//
	modelMatrix *= transform2D::Rotate(RADIANS(wingAngle));
	modelMatrix *= transform2D::Scale(wingScaleX, wingScaleY);
	modelMatrix *= transform2D::MirrorOX();
	meshesToRender.insert(std::make_pair(duckRightWing, modelMatrix));

	if (drawHitbox) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= transform2D::Translate(tailX, tailY);
		modelMatrix *= transform2D::Rotate(realDirectionRadians);
		modelMatrix *= transform2D::Translate(-sizeOx / 2, 0);
		modelMatrix *= transform2D::Scale(sizeOx, sizeOy);
		meshesToRender.insert(std::make_pair(hitbox, modelMatrix));
	}

	return meshesToRender;
}

std::unordered_map<Mesh*, glm::mat3> Duck::moveDuck(float deltaTimeSeconds, float resolutionX, float resolutionY) {
	if (duckState == DUCK_STATE_ACTIVE) {
		timeAlive += deltaTimeSeconds;
		directionChangerCount += deltaTimeSeconds;
		if (directionChangerCount >= 1) {
			directionChangerCount = 0;
			if (!(tailX + getSize().y + speed * deltaTimeSeconds > resolutionX) && !((tailY + getSize().y + speed * deltaTimeSeconds > resolutionY))) {
				int chanceOfChanging = rand() % 101;
				if (chanceOfChanging >= 0 && chanceOfChanging <= 10) {
					bool generate = true;
					while (generate) {
						direction = direction + (rand() % 90);
						if (abs(sin(RADIANS(direction))) <= 0.8 && abs(cos(RADIANS(direction))) <= 0.8)
							generate = false;
					}
				}
			}
		}
		if (timeAlive >= 5) {
			duckState = DUCK_STATE_EVADING;
		}
		else {
			glm::vec2 beakPeakRealCoords = getBeakPeakRealCoordinates();
			if (beakPeakRealCoords[1] >= resolutionY || beakPeakRealCoords[1] <= 0) {
				tailY = beakPeakRealCoords[1];
				tailX = beakPeakRealCoords[0];
				direction = 360 - direction;
			}
			if (beakPeakRealCoords[0] >= resolutionX || beakPeakRealCoords[0] <= 0) {
				tailY = beakPeakRealCoords[1];
				tailX = beakPeakRealCoords[0];
				if (getCircleQuadrant() == 1 || getCircleQuadrant() == 2)
					direction = 180 - direction;
				else 
					direction = 360 - direction + 180;
			}
			if (duckState == DUCK_STATE_ACTIVE) {
				tailX += (speed * deltaTimeSeconds) * cos(RADIANS(direction));
				tailY += (speed * deltaTimeSeconds) * sin(RADIANS(direction));
			}
		}
	}

	if (duckState == DUCK_STATE_EVADING) {
		direction = 90;
		tailY += deltaTimeSeconds * speed / 1.5;
		timeAlive = 0;
		if (tailY >= resolutionY) duckState = DUCK_STATE_EVADED;
	}

	// if duckState -> shot
	if (duckState == DUCK_STATE_SHOT) {
		direction = 270;
		tailY -= deltaTimeSeconds * speed / 2;
		timeAlive = 0;
		if (tailY <= 0) duckState = DUCK_STATE_DEAD;
	}

	updateDuckProperties();
	fly(deltaTimeSeconds);
	
	return drawDuck();
}

void Duck::fly(float deltaTimeSeconds) {
	if (duckState == DUCK_STATE_SHOT || duckState == DUCK_STATE_DEAD) return;

	int factor = 1;
	if (wingState == WING_STATE_IN) {
		if (wingAngle <= defaultWingAngle) {
			wingState = WING_STATE_OUT;
			factor = 1;
		}
		else {
			factor = -1;
		}
	}
	else if (wingState == WING_STATE_OUT) {
		if (wingAngle >= 2 * defaultWingAngle) {
			wingState = WING_STATE_IN;
			factor = -1;
		}
		else {
			factor = 1;
		}
	}
	wingAngle = DEGREES(RADIANS(wingAngle) + factor * deltaTimeSeconds * speed / 500);
}

int Duck::getCircleQuadrant() {
	if (direction <= 90 && direction >= 0) {
		return 1;
	}
	if (direction <= 180 && direction >= 90) {
		return 2;
	}
	if (direction <= 260 && direction >= 180) {
		return 3;
	}
	return 4;
}

bool checkPoint(glm::vec2 point1, glm::vec2 point2, glm::vec2 ref, int checkingSide) {
	float slope = (point2.y - point1.y) / (point2.x - point1.x);
	float slope_term = point1.x;
	float free_term = point1.y;
	float teoretical_y = slope * (ref.x - slope_term) + free_term;
	float delta = ref.y - teoretical_y;
	return (delta * checkingSide) >= 0;
}

bool Duck::checkForCollision(int collisionX, int collisionY) {
	float sizeOx = getSize().x;

	glm::vec2 collisionPosition = { collisionX, collisionY };

	// getting real coordinates of a hitbox rectangle
	glm::vec2 bottomRight = { (cos(RADIANS(direction - 90)) * sizeOx / 2) + tailX, 
								(sin(RADIANS(direction - 90)) * sizeOx / 2) + tailY };
	glm::vec2 bottomLeft = { (cos(RADIANS(direction - 90 + 180)) * sizeOx / 2) + tailX,
							(sin(RADIANS(direction - 90 + 180)) * sizeOx / 2) + tailY };
	glm::vec2 upperRight = { (cos(RADIANS(direction - 90)) * sizeOx / 2) + getBeakPeakRealCoordinates()[0], 
							(sin(RADIANS(direction - 90)) * sizeOx / 2) + getBeakPeakRealCoordinates()[1] };
	glm::vec2 upperLeft = { (cos(RADIANS(direction - 90 + 180)) * sizeOx / 2) + getBeakPeakRealCoordinates()[0], 
							(sin(RADIANS(direction - 90 + 180)) * sizeOx / 2) + getBeakPeakRealCoordinates()[1] };
	bool res = true;

	// check for collision depending of the duck direction angle
	if (getCircleQuadrant() == 1) {
		res &= checkPoint(bottomRight, bottomLeft, collisionPosition, 1);
		res &= checkPoint(bottomLeft, upperLeft, collisionPosition, -1);
		res &= checkPoint(bottomRight, upperRight, collisionPosition, 1);
		res &= checkPoint(upperLeft, upperRight, collisionPosition, -1);
	}
	else if (getCircleQuadrant() == 2) {
		res &= checkPoint(bottomRight, bottomLeft, collisionPosition, 1);
		res &= checkPoint(bottomLeft, upperLeft, collisionPosition, 1);
		res &= checkPoint(bottomRight, upperRight, collisionPosition, -1);
		res &= checkPoint(upperLeft, upperRight, collisionPosition, -1);
	}
	else if (getCircleQuadrant() == 3) {
		res &= checkPoint(bottomRight, bottomLeft, collisionPosition, -1);
		res &= checkPoint(bottomLeft, upperLeft, collisionPosition, 1);
		res &= checkPoint(bottomRight, upperRight, collisionPosition, -1);
		res &= checkPoint(upperLeft, upperRight, collisionPosition, 1);
	}
	else {
		res &= checkPoint(bottomRight, bottomLeft, collisionPosition, -1);
		res &= checkPoint(bottomLeft, upperLeft, collisionPosition, -1);
		res &= checkPoint(bottomRight, upperRight, collisionPosition, 1);
		res &= checkPoint(upperLeft, upperRight, collisionPosition, 1);
	}

	return res;
}

/*
 * used getters and setters
 */

int Duck::getDuckState() { return this->duckState; }
float Duck::getScoreForDuck() { return this->scoreForDuck; }
float Duck::getDirection() { return this->direction; }
float Duck::getSpeed() { return this->speed; }
float Duck::getDefaultSpeed() { return this->defaultSpeed; }
float Duck::getDefaultScoreForDuck() { return this->defaultScoreForDuck; }

bool Duck::getDrawHitbox() { return this->drawHitbox; }


float Duck::getDefaultTailScale() { return this->defaultTailScale; }
float Duck::getTailX() { return this->tailX; }
float Duck::getTailY() { return this->tailY; }
float Duck::getTailScaleX() { return this->tailScaleX; }
float Duck::getTailScaleY() { return this->tailScaleY; }
float Duck::getDefaultWingAngle() { return this->defaultWingAngle; }

void Duck::setDuckState(int duckState) { this->duckState = duckState; }
void Duck::setScoreForDuck(float scoreForDuck) { this->scoreForDuck = scoreForDuck; }
void Duck::setDirection(float direction) { this->direction = direction; }
void Duck::setSpeed(float speed) { this->speed = speed; }
void Duck::setDefaultSpeed(float defaultSpeed) { this->defaultSpeed = defaultSpeed; }
void Duck::setDefaultScoreForDuck(float defaultScoreForDuck) { this->defaultScoreForDuck = defaultScoreForDuck; }

void Duck::setDrawHitbox(bool drawHitbox) { this->drawHitbox = drawHitbox; }

void Duck::setTailX(float tailX) { this->tailX = tailX; }
void Duck::setTailY(float tailY) { this->tailY = tailY; }
void Duck::setTailScaleX(float tailScaleX) { this->tailScaleX = tailScaleX; }
void Duck::setTailScaleY(float tailScaleY) { this->tailScaleY = tailScaleY; }
void Duck::setDefaultWingAngle(float defaultWingAngle) { this->defaultWingAngle = defaultWingAngle; }
void Duck::setDefaultTailScale(float defaultTailScale) { this->defaultTailScale = defaultTailScale; }

/*
 * non used getters and setters
 */

//int Duck::getWingAngle() { return this->wingAngle; }
//float Duck::getBodyX() { return this->bodyX; }
//float Duck::getBodyY() { return this->bodyY; }
//float Duck::getBodyScaleX() { return this->bodyScaleX; }
//float Duck::getBodyScaleY() { return this->bodyScaleY; }
//float Duck::getWingScaleX() { return this->wingScaleX; }
//float Duck::getWingScaleY() { return this->wingScaleY; }
//float Duck::getLeftWingX() { return this->leftWingX; }
//float Duck::getLeftWingY() { return this->leftWingY; }
//float Duck::getRightWingX() { return this->rightWingX; }
//float Duck::getRightWingY() { return this->rightWingY; }
//float Duck::getBeakX() { return this->beakX; }
//float Duck::getBeakY() { return this->beakY; }
//float Duck::getBeakScaleX() { return this->beakScaleX; }
//float Duck::getBeakScaleY() { return this->beakScaleY; }
//float Duck::getHeadX() { return this->headX; }
//float Duck::getHeadY() { return this->headY; }
//float Duck::getHeadScaleX() { return this->headScaleX; }
//float Duck::getHeadScaleY() { return this->headScaleY; }

//void Duck::setWingAngle(float wingAngle) { this->wingAngle = wingAngle; }
//void Duck::setBodyX(float bodyX) { this->bodyX = bodyX; }
//void Duck::setBodyY(float bodyY) { this->bodyY = bodyY; }
//void Duck::setBodyScaleX(float bodyScaleX){ this->bodyScaleX = bodyScaleX; }
//void Duck::setBodyScaleY(float bodyScaleY){ this->bodyScaleY = bodyScaleY; }
//void Duck::setWingScaleX(float wingScaleX) { this->wingScaleX = wingScaleX; }
//void Duck::setWingScaleY(float wingScaleY) { this->wingScaleY = wingScaleY; }
//void Duck::setLeftWingX(float leftWingX) { this->leftWingX = leftWingX; }
//void Duck::setLeftWingY(float leftWingY) { this->leftWingY; }
//void Duck::setRightWingX(float rightWingX) { this->rightWingX = rightWingX; }
//void Duck::setRightWingY(float rightWingY) { this->rightWingY = rightWingY; }
//void Duck::setHeadX(float headX) { this->headX = headX; }
//void Duck::setHeadY(float headY) { this->headY = headY; }
//void Duck::setHeadScaleX(float headScaleX) { this->headScaleX = headScaleX; }
//void Duck::setHeadScaleY(float headScaleY) { this->headScaleY = headScaleY; }
//void Duck::setBeakX(float beakX) { this->headScaleY = beakX; }
//void Duck::setBeakY(float beakY) { this->beakY = beakY; }
//void Duck::setBeakScaleX(float beakScaleX) { this->beakScaleX = beakScaleX; }
//void Duck::setBeakScaleY(float beakScaleY) { this->beakScaleY = beakScaleY; }
