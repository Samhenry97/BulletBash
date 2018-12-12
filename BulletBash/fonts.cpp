#include "fonts.h"

namespace Fonts {
	std::unordered_map<std::string, sf::Font> fonts;

	const sf::Font &get(std::string name) {
		if (fonts.find(name) == fonts.end()) {
			sf::Font font;
			font.loadFromFile("fonts/" + name);
			fonts[name] = font;
		}

		return fonts[name];
	}
}