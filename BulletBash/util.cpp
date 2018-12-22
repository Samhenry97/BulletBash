#include "util.h"

sf::RenderWindow *window = nullptr;
sf::RenderTexture *lights = nullptr;
sf::RectangleShape light;
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

// https://stackoverflow.com/questions/16613616/work-out-whether-to-turn-clockwise-or-anticlockwise-from-two-angles
float turnAngle(float current, float target) {
	float pi2 = 2 * PI;
	target -= current;
	current = 0;
	while (target < 0) { target += pi2; }
	while (target > pi2) { target -= pi2; }
	return target;
}