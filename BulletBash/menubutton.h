#pragma once

#include "gameobject.h"

class MenuButton : public GameObject {
private:
	sf::Text text;
	std::string content;

public:
	bool selected;

	MenuButton(std::string content);
	void select();
	void deselect();
	void update();
	void render();
	void renderStatic();
};

