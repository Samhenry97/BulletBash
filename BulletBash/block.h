#pragma once
#include "gameobject.h"
class Block : public GameObject {
private:
	float y, x;

public:
	Block(float y, float x);
	void update();
	void render();
	void renderMinimap();
};

