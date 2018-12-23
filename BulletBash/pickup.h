#pragma once

#include "const.h"
#include "gameobject.h"

class Pickup : public GameObject {
protected:
	bool interacted;
	bool finished;
	bool highlighted;
	float scale, scaleSpeed;
	int scaleDir;

public:
	Pickup();
	Pickup(vec2 pos);
	virtual void update();
	virtual void render();
	virtual bool interact(GameObject *player);
	bool dead();
	bool interactable();
	void highlight();
};

class PAmmo : public Pickup {
public:
	PAmmo(vec2 pos);
	bool interact(GameObject *player);
};

class PHealth : public Pickup {
public:
	PHealth(vec2 pos);
	bool interact(GameObject *player);
};

class PChest : public Pickup {
private:
	int tier;

public:
	PChest(vec2 pos, int tier);
	bool interact(GameObject *player);
};

class PDoor : public Pickup {
private:
	int dir;

public:
	PDoor(vec2 pos, int dir);
	bool interact(GameObject *player);
	void enable();
};

class PNextFloor : public Pickup {
public:
	PNextFloor(vec2 pos);
	bool interact(GameObject *player);
	void enable();
};