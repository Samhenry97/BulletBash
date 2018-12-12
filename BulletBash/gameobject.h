#pragma once

#include "const.h"

class GameObject {
public:
	sf::RectangleShape sprite;

	GameObject();
	GameObject(vec2 pos, vec2 size);

	virtual void render() = 0;
	virtual void update() = 0;
	virtual void renderStatic();
	virtual void renderMinimap();

	bool intersects(GameObject &other);
	bool contains(vec2 point);
	float dist(GameObject &other);
	float angle(GameObject &other);
	vec2 center();
	void origin();
	void flipLeft();
	void flipRight();
	void flipUp();
	void flipDown();
	virtual int type();
};