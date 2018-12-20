#include "pickup.h"
#include "player.h"
#include "util.h"
#include "sounds.h"

Pickup::Pickup() {

}

Pickup::Pickup(vec2 pos) {
	sprite.setPosition(pos);
}

void Pickup::update() {

}

void Pickup::render() {
	window->draw(sprite);
}

void Pickup::interact(GameObject *player) {
	interacted = true;
}

bool Pickup::dead() {
	return interacted;
}

PAmmo::PAmmo(vec2 pos) : Pickup(pos) {
	sprite.setTexture(Images::get("ammo.png"));
	sprite.setSize(vec2(ENT_SIZE / 2, ENT_SIZE / 2));
}

void PAmmo::interact(GameObject *player) {
	Pickup::interact(player);
	Sounds::play("ammo.wav");
	if (Player *p = dynamic_cast<Player*>(player)) {
		p->fillAmmo();
	}
}

PHealth::PHealth(vec2 pos) : Pickup(pos) {
	sprite.setTexture(Images::get("health.png"));
	sprite.setSize(vec2(ENT_SIZE / 2, ENT_SIZE / 2));
}

void PHealth::interact(GameObject *player) {
	Pickup::interact(player);
	if (Player *p = dynamic_cast<Player*>(player)) {
		p->heal(100);
	}
}