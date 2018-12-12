#pragma once

#include <unordered_map>

#include "const.h"

namespace Images {
	extern std::unordered_map<std::string, sf::Texture> textures;
	sf::Texture *get(std::string name);
}