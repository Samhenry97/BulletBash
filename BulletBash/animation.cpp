#include "animation.h"
#include "util.h"

Animation::Animation(sf::RectangleShape *sprite, vec2 spriteSize, int totalFrames, float animTime) {
	this->sprite = sprite;
	this->spriteSize = spriteSize;
	this->totalFrames = totalFrames;
	this->animTime = animTime;
	this->animClock = 0.0f;
	setFrame(0);
}

void Animation::start() {
	running = true;
	animClock = 0.0f;
	times = 0;
}

void Animation::stop() {
	running = false;
}

void Animation::update() {
	if (!running) return;
	updateFrame();

	animClock += frameTime;
	if (animClock >= animTime) {
		times++;
		if (maxTimes != -1 && times >= maxTimes) {
			running = false;
			animClock = animTime;
		} else {
			animClock = 0;
		}
	}
}

void Animation::setTimes(int times) {
	maxTimes = times;
}

void Animation::setFrame(int frame) {
	float percent = (float) frame / (float) totalFrames;
	animClock = animTime * percent;
	updateFrame();
}

void Animation::updateFrame() {
	float percent = animClock / animTime;
	int frame = std::min((int)(percent * (float)totalFrames), totalFrames - 1);
	sprite->setTextureRect(sf::IntRect(frame * spriteSize.x, 0, spriteSize.x, spriteSize.y));
}