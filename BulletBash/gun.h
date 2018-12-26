#pragma once

#include "const.h"
#include "gameobject.h"
#include "pickup.h"

class Gun : public Pickup {
protected:
	GameObject *owner;

public:
	bool reloading = false;
	std::string name;
	float fireClock, fireTime;
	float reloadClock, reloadTime;
	float speed, angle, spread;
	int clip, ammo, activeAmmo, startingAmmo;

	Gun(GameObject *owner, float fireTime, float reloadTime, int clip, int ammo);

	virtual void fire();
	virtual void update();
	virtual void render();

	void tryFire();
	void reload();
	void setOwner(GameObject *owner);
	void drop();
	float getAngle();
	bool interact(GameObject *player);
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

class GDoubleGun : public Gun {
public:
	GDoubleGun(GameObject *owner);
	void fire();
};

class GSplashGun : public Gun {
public:
	GSplashGun(GameObject *owner);
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

class GRay : public Gun {
public:
	GRay(GameObject *owner);
	void fire();
};