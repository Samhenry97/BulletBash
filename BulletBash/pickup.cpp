#include "pickup.h"
#include "player.h"
#include "util.h"
#include "sounds.h"

Pickup::Pickup() : Pickup(vec2(0, 0)) { }

Pickup::Pickup(vec2 pos) {
	sprite.setPosition(pos);
	sprite.setOutlineColor(sf::Color::White);
	scale = 1.0f;
	scaleSpeed = 2.0f;
	scaleDir = 1;
}

void Pickup::update() {
	GameObject::update();
	if (scaleDir == 1) {
		scale += scaleSpeed * frameTime;
		if (scale >= 2.0f) { scaleDir = -scaleDir; }
	} else {
		scale -= scaleSpeed * frameTime;
		if (scale <= 1.0f) { scaleDir = -scaleDir; }
	}
	sprite.setScale(scale, scale);
}

void Pickup::render() {
	window->draw(sprite);
	if (highlighted) {
		highlighted = false;
		sprite.setOutlineThickness(0.0f);
	}
}

void Pickup::highlight() {
	highlighted = true;
	sprite.setOutlineThickness(3.0f);
}

bool Pickup::interact(GameObject *player) {
	interacted = true;
	return true;
}

bool Pickup::dead() {
	return finished;
}

bool Pickup::interactable() {
	return !interacted;
}

PAmmo::PAmmo(vec2 pos) : Pickup(pos) {
	sprite.setTexture(Images::get("ammo.png"));
	sprite.setSize(vec2(ENT_SIZE / 2, ENT_SIZE / 2));
	origin();
}

bool PAmmo::interact(GameObject *player) {
	Pickup::interact(player);
	finished = true;
	Sounds::play("ammo.wav");
	if (Player *p = dynamic_cast<Player*>(player)) {
		p->fillAmmo();
	}
	return true;
}

PHealth::PHealth(vec2 pos) : Pickup(pos) {
	sprite.setTexture(Images::get("health.png"));
	sprite.setSize(vec2(ENT_SIZE / 2, ENT_SIZE / 2));
	origin();
}

bool PHealth::interact(GameObject *player) {
	Pickup::interact(player);
	finished = true;
	if (Player *p = dynamic_cast<Player*>(player)) {
		p->heal(100);
	}
	return true;
}

PChest::PChest(vec2 pos, int tier) : Pickup(pos) {
	this->tier = tier;
	sprite.setTexture(Images::get("chest/chest" + STR(tier) + ".png"));
	sprite.setSize(vec2(BLOCK_SIZE, BLOCK_SIZE));
	scaleSpeed = 0.0f;
}

bool PChest::interact(GameObject *player) {
	if (!interacted) {
		Sounds::play("amazing.wav");
		interacted = true;
		Gun *g = new GMinigun(nullptr);
		g->sprite.setPosition(sprite.getPosition());
		game->room->addItem(g);
		sprite.setTexture(Images::get("chest/chest" + STR(tier) + "open.png"));
	}
	return false;
}

PDoor::PDoor(vec2 pos, int dir) : Pickup(pos), dir(dir) {
	sprite.setTexture(Images::get("teleporter.png"));
	sprite.setSize(vec2(BLOCK_SIZE, BLOCK_SIZE));
	scaleSpeed = 0.0f;
	interacted = true;
}

bool PDoor::interact(GameObject *player) {
	if (!interacted) {
		game->transport(dir);
	}
	return false;
}

void PDoor::enable() {
	sprite.setTexture(Images::get("teleporteropen.png"));
	interacted = false;
}

PNextFloor::PNextFloor(vec2 pos) : Pickup(pos) {
	animation = new Animation(&sprite, vec2(200, 300), 6, 2.0f);
	animation->setTimes(1);
	animation->stop();
	sprite.setSize(vec2(200, 300));
	sprite.setOrigin(100, 150);
	sprite.setTexture(Images::get("nextfloor.png"));
	scaleSpeed = 0.0f;
	interacted = true;
}

bool PNextFloor::interact(GameObject *player) {
	if (!interacted) {
		game->nextFloor();
	}
	return false;
}

void PNextFloor::enable() {
	animation->start();
	interacted = false;
}