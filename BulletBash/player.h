#pragma once

#include "const.h"
#include "gameobject.h"
#include "xbox.h"
#include "gun.h"

class Player : public GameObject {
private:
	Xbox *xbox;
	float dashClock, dashTime, dashAngle;
	float damageClock, damageTime;
	float animClock, animTime;
	float dropClock, dropTime;
	int animFrame;
	bool damaging, dropping;
	std::vector<Gun*> guns;
	sf::RectangleShape gui;
	sf::RectangleShape reloadOutline;
	sf::RectangleShape reloadProgress;
	Gun *gun;

public:
	int health, maxHealth, dir, controller;
	float moveAngle, baseSpeed, speed;;
	bool dashing;
	std::string image;

	Player(vec2 pos, vec2 size, std::string image);
	void render();
	void renderStatic();
	void renderMinimap();
	void update();
	void damage(int amt);
	void addItem(GameObject *item);
	void switchGun(bool remove);
	void buttonPressed(int button);
	void buttonReleased(int button);
	void axisMoved(int axis, int pos);
	void changeImage(std::string newImage);
	void fillAmmo();
	void heal(int amt);
	int type();
	void setController(int newController);
	bool dead();
};