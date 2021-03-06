#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <sstream>
#include <queue>
#include <iostream>
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
#define MINIMAP_ROOM_SIZE 400.0f
#define MINIMAP_ROOM_DRAW MINIMAP_ROOM_SIZE * 0.8f
#define MINIMAP_ROOM_PAD MINIMAP_ROOM_SIZE * 0.2f
#define MINIMAP_X 0.1f
#define MINIMAP_Y 0.1f
#define ENT_SIZE 50
#define BLOCK_SIZE 64
#define NAME "Bullet Bash"

#define BULLET_PLAYER 0
#define BULLET_ENEMY 1

#define COLLISION_NONE 0b0
#define COLLISION_ENEMY 0b1
#define COLLISION_PLAYER 0b10
#define COLLISION_BLOCK 0b100

#define CONTACT_DAMAGE 10

#define MAX_ANIM 2
#define MAX_CHARACTER 6
#define MAX_ROOM_BACK 5
#define WORLD_RADIUS 5
#define MAX_ROOM_LEVEL WORLD_RADIUS - 1
#define RADIANS_TO_DEGREES (float) (180 / 3.14)
#define SAFEZONE 4

#define DIR_UP 0
#define DIR_RIGHT 1
#define DIR_DOWN 2
#define DIR_LEFT 3

#define STR(x) (std::to_string(x))
#define INF 0x7FFFFFFF
#define PI 3.14159265359

#define BACK_MUSIC 4
#define BOSS_MUSIC 2

typedef sf::Vector2f vec2;
typedef sf::Vector2u vec2u;
typedef sf::Vector2i vec2i;
typedef std::pair<int, int> pi;

struct RayCastResult {
	vec2 pos;
	bool collision;
};