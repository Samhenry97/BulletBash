#pragma once
#include "gameobject.h"

class Wall : public GameObject {
private:
	float y, x;

public:
	Wall(vec2 pos);
	virtual void update();
	virtual void render();
	virtual void renderMinimap();
};

class WFire : public Wall {
public:
	WFire(vec2 pos);
	void render();
};