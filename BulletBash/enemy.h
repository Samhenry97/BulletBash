#pragma once
#include "gameobject.h"
#include "gun.h"

class Enemy : public GameObject {
protected:
	float speed, baseSpeed;
	int health, maxHealth, hitDamage;
	float damageClock, damageTime;
	float knockAngle, knockSpeed;
	float spawnClock, spawnTime;
	float pathClock, pathTime;
	std::vector<vec2i> path;
	sf::RectangleShape gui;
	Gun *gun;

public:
	bool damaging, spawning, knocking, smart;

	Enemy(vec2 pos);
	virtual ~Enemy();
	void update();
	void render();
	void renderMinimap();
	void pathfind();
	virtual void damage(int amt);
	bool dead();
};

class EBasic : public Enemy {
public:
	EBasic(vec2 pos);
};

class EAlien : public Enemy {
public:
	EAlien(vec2 pos);
};

class ESlime : public Enemy {
private:
	int level;

public:
	ESlime(vec2 pos);
	ESlime(vec2 pos, int level);
	~ESlime();
};

class Boss : public Enemy {
public:
	Boss(vec2 pos);
	void renderStatic();
};

class EEyeBoss : public Boss {
private:
	float expClock, expTime, expAngle;
	int expCount, expTotal;
	bool exping;

public:
	EEyeBoss(vec2 pos);
	void update();
};

class ERamBoss : public Boss {
private:
	float chargeAngle, chargeDist, chargeMax, chargeSpeed;
	float restClock, restTime;
	bool charging = false;

public:
	ERamBoss(vec2 pos);
	void update();
	void damage(int amt);
};

template<typename T> Enemy* CREATE_ENEMY(vec2 pos) { return new T(pos); }
extern std::vector<Enemy*(*)(vec2 pos)> NORMAL_ENEMIES;
extern std::vector<Enemy*(*)(vec2 pos)> BOSS_ENEMIES;