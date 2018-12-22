#include "animation.h"
#include "util.h"

Animation::Animation(sf::RectangleShape *sprite, vec2 spriteSize, int totalFrames, float animTime) {
	this->sprite = sprite;
	this->spriteSize = spriteSize;
	this->totalFrames = totalFrames;
	this->animTime = animTime;
	this->animClock = 0.0f;
}

void Animation::update() {
	float percent = animClock / animTime;
	int frame = percent * (float) totalFrames;
	sprite->setTextureRect(sf::IntRect(frame * spriteSize.x, 0, spriteSize.x, spriteSize.y));

	animClock += frameTime;
	if (animClock >= animTime) {
		animClock = 0;
	}
}