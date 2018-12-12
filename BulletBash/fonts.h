#pragma once

#include "const.h"

namespace Fonts {
	extern std::unordered_map<std::string, sf::Font> fonts;
	const sf::Font &get(std::string name);
}