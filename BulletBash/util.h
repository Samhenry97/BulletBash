#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "game.h"

extern sf::RenderWindow *window;
extern sf::View *view;
extern sf::View *minimap;
extern Game *game;
extern float frameTime;
extern bool running;
extern int totalPlayers;

float randFloat(float a, float b);
float turnAngle(float current, float target);