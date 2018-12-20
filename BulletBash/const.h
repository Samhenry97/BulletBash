#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <sstream>
#include <queue>
#include <algorithm>
#include <functional>

#include "images.h"
#include "logger.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define WINDOW_MIN_WIDTH 1920
#define WINDOW_MIN_HEIGHT 1080
#define WINDOW_MAX_WIDTH 2560
#define WINDOW_MAX_HEIGHT 1440
#define MINIMAP_WIDTH 0.8f
#define MINIMAP_HEIGHT 0.8f
#define MINIMAP_X 0.1f
#define MINIMAP_Y 0.1f
#define ENT_SIZE 50
#define BLOCK_SIZE 64
#define NAME "Bullet Bash"

#define BULLET_PLAYER 0
#define BULLET_ENEMY 1

#define COLLISION_NONE 0
#define COLLISION_ENEMY 1
#define COLLISION_PLAYER 2
#define COLLISION_BLOCK 3

#define MAX_ANIM 2
#define MAX_CHARACTER 5
#define MAX_ROOM_BACK 5
#define RADIANS_TO_DEGREES (float) (180 / 3.14)
#define SAFEZONE 4

#define STR(x) (std::to_string(x))
#define INF 0x7FFFFFFF
#define PI 3.14159265359

#define BACK_MUSIC 4
#define BOSS_MUSIC 2

typedef sf::Vector2f vec2;
typedef sf::Vector2u vec2u;
typedef sf::Vector2i vec2i;
typedef std::pair<int, int> pi;