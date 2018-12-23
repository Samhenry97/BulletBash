#pragma once

#include "const.h"

class Animation {
protected:
	sf::RectangleShape *sprite;
	vec2 spriteSize;
	int curFrame, totalFrames;
	float animClock, animTime;
	int times = 0, maxTimes = -1; // Default to infinite animations
	bool running = true; // Is it currently running?

public:
	Animation(sf::RectangleShape *sprite, vec2 spriteSize, int totalFrames, float animTime);
	void start();
	void stop();
	void update();
	void setTimes(int times);
	void setFrame(int frame);
	void updateFrame();
};

class GridAnimation : public Animation {

};