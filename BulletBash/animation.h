#pragma once

#include "const.h"

class Animation {
protected:
	sf::RectangleShape *sprite;
	vec2 spriteSize;
	int curFrame, totalFrames;
	float animClock, animTime;

public:
	Animation(sf::RectangleShape *sprite, vec2 spriteSize, int totalFrames, float animTime);
	void update();
};

class GridAnimation : public Animation {

};