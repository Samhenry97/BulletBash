#pragma once

#include "const.h"
#include "animation.h"

class GameObject {
protected:
	Animation *animation = nullptr;

public:
	sf::RectangleShape sprite;
	sf::RectangleShape hitbox;
	bool solid = true;

	GameObject();
	GameObject(vec2 pos, vec2 size);
	~GameObject();

	virtual void render() = 0;
	virtual void update();
	virtual void renderStatic();
	virtual void renderMinimap();
	virtual bool intersects(GameObject *other);
	virtual bool contains(vec2 point);

	void setPosition(vec2 newPos);
	void setSize(vec2 newSize);
	void setRotation(float newRotation);
	void setOrigin(vec2 newOrigin);
	void move(vec2 offset);

	float dist(GameObject *other);
	float angle(GameObject *other);
	vec2 center();
	void origin();
	void flipLeft();
	void flipRight();
	void flipUp();
	void flipDown();
	virtual int type();
	std::vector<vec2> transformPoints();
};