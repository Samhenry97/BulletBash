#pragma once

#include "const.h"
#include "gameobject.h"

class Pickup : public GameObject {
private:
	bool interacted;

public:
	Pickup();
	Pickup(vec2 pos);
	virtual void update();
	virtual void render();
	virtual void interact(GameObject *player);
	bool dead();
};

class PAmmo : public Pickup {
public:
	PAmmo(vec2 pos);
	void interact(GameObject *player);
};

class PHealth : public Pickup {
public:
	PHealth(vec2 pos);
	void interact(GameObject *player);
};