#include "gun.h"
#include "game.h"
#include "sounds.h"
#include "fonts.h"
#include "util.h"

Gun::Gun(GameObject *owner, float fireTime, float reloadTime, int clip, int ammo) {
	this->owner = owner;
	this->fireTime = fireTime;
	this->fireClock = fireTime;
	this->reloadTime = reloadTime;
	this->clip = clip;
	this->ammo = ammo;
	this->startingAmmo = ammo;
}

void Gun::tryFire() {
	if (!reloading && fireClock >= fireTime) {
		if (activeAmmo <= 0 && (clip != -1 || ammo != -1)) {
			reload();
		} else {
			fireClock = 0;
			activeAmmo--;

			fire();
		}
	}
}

void Gun::fire() {

}

void Gun::reload() {
	if (ammo == 0) {
		Sounds::play("empty.wav");
	} else if (!reloading && (activeAmmo < clip || clip == -1)) {
		reloading = true;
		reloadClock = 0;

		Sounds::play("reload.wav");
	}
}

void Gun::update() {
	if (reloading && reloadClock >= reloadTime) {
		reloading = false;
		if (ammo != -1) {
			int adding = clip == -1 ? ammo : std::min(ammo, clip - activeAmmo);
			ammo -= adding;
			activeAmmo += adding;
		} else {
			activeAmmo = clip;
		}

		fireClock = fireTime;
	}

	fireClock += frameTime;
	reloadClock += frameTime;
}

float Gun::getAngle() {
	float shift = randFloat(0.0f, spread) - spread / 2;
	return angle + shift;
}

void Gun::drop() {
	owner = nullptr;
	angle = 0;
	game->room->addItem(this);
}

void Gun::setOwner(GameObject *owner) {
	this->owner = owner;
}

void Gun::render() {
	sf::Vector2u size = sprite.getTexture()->getSize();
	float degrees = angle * RADIANS_TO_DEGREES;
	if (degrees < -90 || degrees > 90) {
		sprite.setTextureRect(sf::IntRect(0, size.y, size.x, -(int) size.y));
	} else {
		sprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y));
	}

	if(owner) sprite.setPosition(owner->center());
	sprite.setRotation(angle * RADIANS_TO_DEGREES);
	Pickup::render();
}

bool Gun::interact(GameObject *player) {
	if (Player *p = dynamic_cast<Player*>(player)) {
		p->addItem(this);
	}
	return true;
}

GPistol::GPistol(GameObject *owner) : Gun(owner, 0.2f, 1.0f, 12, -1) {
	sprite.setTexture(Images::get("pistol.png"));
	sprite.setSize(vec2(40, 30));
	speed = 600.0f;
	spread = 0.32f;
	origin();
	name = "Pistol";
}

void GPistol::fire() {
	Sounds::play("bullet.wav");
	game->room->addBullet(new BBasic(owner->type(), owner->center(), speed, getAngle()));
}

GMinigun::GMinigun(GameObject *owner) : Gun(owner, 0.08f, 0, -1, 10000) {
	sprite.setTexture(Images::get("minigun.png"));
	sprite.setSize(vec2(100, 30));
	speed = 1000.0f;
	origin();
	name = "Minigun";
}

void GMinigun::fire() {
	Sounds::play("bullet.wav");
	game->room->addBullet(new BBasic(owner->type(), owner->center(), speed, getAngle()));
}

GShotgun::GShotgun(GameObject *owner) : Gun(owner, 0.6f, 1.4f, 6, 200) {
	sprite.setTexture(Images::get("shotgun.png"));
	sprite.setSize(vec2(80, 25));
	speed = 800.0f;
	spread = 0.64f;
	origin();
	name = "Shotgun";
}

void GShotgun::fire() {
	Sounds::play("shotgun.wav");
	for (int i = 0; i < 8; i++) {
		game->room->addBullet(new BHeavy(owner->type(), owner->center(), speed, getAngle()));
	}
}

GBubble::GBubble(GameObject *owner) : Gun(owner, 1.5f, 2.0f, 5, 50) {
	sprite.setTexture(Images::get("bubblegun.png"));
	sprite.setSize(vec2(50, 30));
	speed = 400.0f;
	origin();
	name = "Bubble Gun";
}

void GBubble::fire() {
	Sounds::play("shotgun.wav");
	game->room->addBullet(new BBubble(owner->type(), owner->center(), speed, getAngle()));
}

GGunGun::GGunGun(GameObject *owner) : Gun(owner, 0.6f, 1.2f, 6, 120) {
	sprite.setTexture(Images::get("gungun.png"));
	sprite.setSize(vec2(75, 25));
	speed = 800.0f;
	origin();
	name = "Gun Gun";
}

void GGunGun::fire() {
	Sounds::play("shotgun.wav");
	game->room->addBullet(new BGun(owner->type(), owner->center(), speed, getAngle()));
}

GSteveGun::GSteveGun(GameObject *owner) : Gun(owner, 2.0f, 3.0f, 6, 120) {
	sprite.setTexture(Images::get("stevegun.png"));
	sprite.setSize(vec2(100, 50));
	speed = 800.0f;
	origin();
	name = "Steve Gun";
}

void GSteveGun::fire() {
	Sounds::play("shotgun.wav");
	game->room->addBullet(new BSteve(owner->type(), owner->center(), speed, getAngle()));
}

GFlameThrower::GFlameThrower(GameObject *owner) : Gun(owner, 0.02f, 0, -1, 2000) {
	sprite.setTexture(Images::get("flamethrower.png"));
	sprite.setSize(vec2(80, 40));
	speed = 800.0f;
	spread = 0.32;
	origin();
	name = "Flame Thrower";
}

void GFlameThrower::fire() {
	Sounds::play("flamethrower.wav");
	game->room->addBullet(new BFlame(owner->type(), owner->center(), speed, getAngle()));
}

GHoming::GHoming(GameObject *owner) : Gun(owner, 0.15f, 0.8f, 16, 150) {
	sprite.setTexture(Images::get("hominggun.png"));
	sprite.setSize(vec2(60, 50));
	speed = 600.0f;
	origin();
	name = "Homing Gun";
}

void GHoming::fire() {
	Sounds::play("bullet.wav");
	game->room->addBullet(new BHoming(owner->type(), owner->center(), speed, getAngle()));
}