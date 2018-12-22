#pragma once

#include "const.h"
#include "animation.h"

class GameObject {
protected:
	Animation *animation = nullptr;

public:
	sf::RectangleShape sprite;

	GameObject();
	GameObject(vec2 pos, vec2 size);
	~GameObject();

	virtual void render() = 0;
	virtual void update();
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