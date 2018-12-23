#include "bullet.h"
#include "game.h"
#include "util.h"

Bullet::Bullet(int type, vec2 pos, float speed, float angle) {
	sprite.setPosition(pos);
	sprite.setRotation(angle * RADIANS_TO_DEGREES);
	this->speed = speed;
	this->moveAngle = angle;
	this->type = type;
	maxDist = 100000.0f;
	damage = 20;
	splash = false;
}

void Bullet::update() {
	if (homing) {
		GameObject *nearest = nullptr;
		if (type == BULLET_PLAYER) {
			nearest = game->room->nearestEnemy(this);
		} else if (type == BULLET_ENEMY) {
			nearest = game->nearestPlayer(this);
		}

		if (nearest) {
			float turn = turnAngle(GameObject::angle(*nearest), moveAngle);
			if (GameObject::dist(*nearest) <= homingDeadzone) {
				moveAngle += homingSpeed * frameTime * (turn > PI ? 1 : -1);
			}
		}
	}

	float x = speed * frameTime * cos(moveAngle);
	float y = speed * frameTime * sin(moveAngle);
	vec2 move = vec2(x, y);
	std::vector<vec2> transform = {
		sprite.getTransform().transformPoint(vec2(0, 0)) + move,
		sprite.getTransform().transformPoint(vec2(sprite.getSize().x, 0)) + move,
		sprite.getTransform().transformPoint(vec2(0, sprite.getSize().y)) + move,
		sprite.getTransform().transformPoint(vec2(sprite.getSize().x, sprite.getSize().y)) + move
	};
	for (int i = 0; i < transform.size(); i++) {
		if (game->blockCollision(transform[i])) kill();
	}
	
	dist += hypot(y, x);
	sprite.move(move);
}

void Bullet::render() {
	sprite.setRotation(moveAngle * RADIANS_TO_DEGREES);
	window->draw(sprite);
}

void Bullet::kill() {
	dist = maxDist;
}

bool Bullet::dead() {
	return dist >= maxDist;
}

BBasic::BBasic(int type, vec2 pos, float speed, float angle) : Bullet(type, pos, speed, angle) {
	sprite.setTexture(Images::get("bullet.png"));
	sprite.setSize(vec2(30, 30));
	damage = 20;
	bouncy = true;
	origin();
}

BHeavy::BHeavy(int type, vec2 pos, float speed, float angle) : Bullet(type, pos, speed, angle) {
	sprite.setTexture(Images::get("beam.png"));
	sprite.setSize(vec2(50, 15));
	origin();
	damage = 60;
}

BBubble::BBubble(int type, vec2 pos, float speed, float angle) : Bullet(type, pos, speed, angle) {
	sprite.setTexture(Images::get("explode.png"));
	sprite.setSize(vec2(60, 60));
	origin();
	splash = true;
	damage = 0;
}

void BBubble::update() {
	Bullet::update();
	speed -= 3.0f;
	if (speed <= 0) { kill(); }
}

void BBubble::kill() {
	Bullet::kill();
	float angle = 0;
	for (int i = 0; i < 42; i++) {
		game->room->addBullet(new BBasic(type, center(), 600.0f, angle));
		angle += 0.15f;
	}
}

BGun::BGun(int type, vec2 pos, float speed, float angle) : Bullet(type, pos, speed, angle) {
	sprite.setTexture(Images::get("pistol.png"));
	sprite.setSize(vec2(40, 30));
	sprite.setFillColor(sf::Color::White);
	displayAngle = angle;
	damage = 100;
	shootTime = 0.1f;
}

void BGun::update() {
	Bullet::update();
	displayAngle += 0.2f;
	shootClock += frameTime;
	if (shootClock >= shootTime) {
		game->room->addBullet(new BBasic(type, center(), 800.0f, displayAngle));
		shootClock = 0;
	}
}

void BGun::render() {
	sprite.setRotation(displayAngle * RADIANS_TO_DEGREES);
	Bullet::render();
}

BSteve::BSteve(int type, vec2 pos, float speed, float angle) : Bullet(type, pos, speed, angle) {
	sprite.setTexture(Images::get("steve.png"));
	sprite.setSize(vec2(60, 30));
	sprite.setFillColor(sf::Color::White);
	displayAngle = angle;
	shootTime = 0.15f;
	damage = 400;
}

void BSteve::update() {
	Bullet::update();
	displayAngle += 0.2f;
	shootClock += frameTime;
	if (shootClock >= shootTime) {
		game->room->addBullet(new BGun(type, center(), 800.0f, displayAngle));
		shootClock = 0;
	}
}

void BSteve::render() {
	sprite.setRotation(displayAngle * RADIANS_TO_DEGREES);
	Bullet::render();
}

BFlame::BFlame(int type, vec2 pos, float speed, float angle) : Bullet(type, pos, speed, angle) {
	sprite.setTexture(Images::get("flame.png"));
	sprite.setSize(vec2(120, 40));
	sprite.setFillColor(sf::Color::White);
	origin();
	damage = 2;
	splash = true;
}

BHoming::BHoming(int type, vec2 pos, float speed, float angle) : Bullet(type, pos, speed, angle) {
	sprite.setTexture(Images::get("homing.png"));
	sprite.setSize(vec2(40, 14));
	origin();
	damage = 80;
	homing = true;
	homingSpeed = 2.0f;
	homingDeadzone = 400.0f;
}