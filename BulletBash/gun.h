#pragma once

#include "const.h"
#include "gameobject.h"

class Gun : public GameObject {
protected:
	GameObject *owner;

public:
	bool reloading = false;
	std::string name;
	float fireClock, fireTime;
	float reloadClock, reloadTime;
	float speed, angle;
	int clip, ammo, activeAmmo;

	Gun(GameObject *owner, float fireTime, float reloadTime, int clip, int ammo);

	void tryFire();
	virtual void fire();
	void reload();
	void setOwner(GameObject *owner);
	void update();
	void render();
	void drop();
};

class GPistol : public Gun {
public:
	GPistol(GameObject *owner);
	void fire();
};

class GMinigun : public Gun {
public:
	GMinigun(GameObject *owner);
	void fire();
};

class GShotgun : public Gun {
public:
	GShotgun(GameObject *owner);
	void fire();
};

class GBubble : public Gun {
public:
	GBubble(GameObject *owner);
	void fire();
};

class GGunGun : public Gun {
public:
	GGunGun(GameObject *owner);
	void fire();
};

class GSteveGun : public Gun {
public:
	GSteveGun(GameObject *owner);
	void fire();
};

class GFlameThrower : public Gun {
public:
	GFlameThrower(GameObject *owner);
	void fire();
};

class GHoming : public Gun {
public:
	GHoming(GameObject *owner);
	void fire();
};