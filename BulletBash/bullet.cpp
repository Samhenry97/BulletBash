#include "bullet.h"
#include "game.h"
#include "util.h"
#include "physics.h"

Bullet::Bullet(int type, vec2 pos, float speed, float angle) {
	setPosition(pos);
	setRotation(angle * RADIANS_TO_DEGREES);
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
			float turn = turnAngle(GameObject::angle(nearest), moveAngle);
			if (GameObject::dist(nearest) <= homingDeadzone) {
				moveAngle += homingSpeed * frameTime * (turn > PI ? 1 : -1);
			}
		}
	}

	float x = speed * frameTime * cos(moveAngle);
	float y = speed * frameTime * sin(moveAngle);
	vec2 offset = vec2(x, y);
	auto transform = transformPoints();
	for (int i = 0; i < transform.size(); i++) {
		if (game->blockCollision(transform[i] + offset)) kill();
	}
	
	dist += hypot(y, x);
	move(offset);
}

void Bullet::render() {
	setRotation(moveAngle * RADIANS_TO_DEGREES);
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
	setSize(vec2(30, 30));
	damage = 20;
	bouncy = true;
	origin();
}

BHeavy::BHeavy(int type, vec2 pos, float speed, float angle) : Bullet(type, pos, speed, angle) {
	sprite.setTexture(Images::get("beam.png"));
	setSize(vec2(50, 15));
	origin();
	damage = 60;
}

BBubble::BBubble(int type, vec2 pos, float speed, float angle) : Bullet(type, pos, speed, angle) {
	sprite.setTexture(Images::get("explode.png"));
	setSize(vec2(60, 60));
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
	sprite.setFillColor(sf::Color::White);
	setSize(vec2(40, 30));
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
	sprite.setFillColor(sf::Color::White);
	setSize(vec2(60, 30));
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
	sprite.setFillColor(sf::Color::White);
	setSize(vec2(120, 40));
	origin();
	damage = 2;
	splash = true;
}

BHoming::BHoming(int type, vec2 pos, float speed, float angle) : Bullet(type, pos, speed, angle) {
	sprite.setTexture(Images::get("homing.png"));
	setSize(vec2(40, 14));
	origin();
	damage = 80;
	homing = true;
	homingSpeed = 2.0f;
	homingDeadzone = 400.0f;
}

BLine::BLine(int type, vec2 pos, float angle) : Bullet(type, pos, 0.0, angle) {
	damage = 10;
	splash = true;
	sprite.setFillColor(sf::Color::Green);
	RayCastResult res = game->room->raycast(hitbox.getPosition(), moveAngle, game->room->raycastLength());
	endPos = res.pos;
	killTime = 0.5f;
}

void BLine::update() {
	animTime += frameTime;
	if (animTime >= killTime) kill();
}

void BLine::render() {
	float len = hypot(endPos.y - sprite.getPosition().y, endPos.x - sprite.getPosition().x);
	sprite.setSize(vec2(len, 5));
	sprite.setRotation(moveAngle * RADIANS_TO_DEGREES);
	window->draw(sprite);
}

bool BLine::intersects(GameObject *other) {
	auto transform = other->transformPoints();
	for (int i = 0; i < 4; i++) {
		if (Physics::lineIntersects(hitbox.getPosition(), endPos, transform[i], transform[(i + 1) % 4])) return true;
	}
	return false;
}