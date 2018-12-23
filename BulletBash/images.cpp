#pragma once

#include "images.h"

namespace Images {
	std::unordered_map<std::string, sf::Texture> textures;

	sf::Texture *get(std::string name) {
		if (textures.find(name) == textures.end()) {
			sf::Texture tex;
			if (!tex.loadFromFile("images/" + name)) {
				Logger::warn("Could not load image: " + name);
			}
			textures[name] = tex;
		}

		return &textures[name];
	}
}