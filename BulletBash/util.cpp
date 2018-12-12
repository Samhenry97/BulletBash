#include "util.h"

sf::RenderWindow *window = nullptr;
sf::View *view = nullptr;
sf::View *minimap = nullptr;
Game *game = nullptr;
float frameTime;
bool running;
int totalPlayers;

float randFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float r = random * (b - a);
	return a + r;
}

// https://math.stackexchange.com/questions/1366869/calculating-rotation-direction-between-two-angles
float turnAngle(float current, float target) {
	float provisional = target - current;
	while (provisional <= -PI) { provisional += PI; }
	while (provisional > PI) { provisional -= PI; }
	return provisional;
}