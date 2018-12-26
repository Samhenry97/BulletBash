#pragma once
#include "gameobject.h"

class Bullet : public GameObject {
protected:
	float speed, moveAngle, homingSpeed, homingDeadzone;
	float maxDist, dist = 0;

public:
	bool splash, homing, bouncy;
	int damage, type;

	Bullet(int type, vec2 pos, float speed, float angle);

	virtual void render();
	virtual void update();
	virtual void kill();
	bool dead();
};

class BBasic : public Bullet {
public:
	BBasic(int type, vec2 pos, float speed, float angle);
};

class BHeavy : public Bullet {
public:
	BHeavy(int type, vec2 pos, float speed, float angle);
};

class BBubble : public Bullet {
public:
	BBubble(int type, vec2 pos, float speed, float angle);
	void update();
	void kill();
};

class BGun : public Bullet {
private:
	float displayAngle, shootClock, shootTime;

public:
	BGun(int type, vec2 pos, float speed, float angle);
	void update();
	void render();
};

class BSteve : public Bullet {
private:
	float displayAngle, shootClock, shootTime;

public:
	BSteve(int type, vec2 pos, float speed, float angle);
	void update();
	void render();
};

class BFlame : public Bullet {
public:
	BFlame(int type, vec2 pos, float speed, float angle);
};

class BHoming : public Bullet {
public:
	BHoming(int type, vec2 pos, float speed, float angle);
};

class BLine : public Bullet {
private:
	vec2 endPos;
	float animTime, killTime;

public:
	BLine(int type, vec2 pos, float angle);
	void update();
	void render();
	bool intersects(GameObject *other);
};