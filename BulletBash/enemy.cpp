#include "enemy.h"
#include "game.h"
#include "player.h"
#include "sounds.h"
#include "const.h"
#include "particle.h"
#include "util.h"

std::vector<Enemy*(*)(vec2 pos)> NORMAL_ENEMIES;
std::vector<Enemy*(*)(vec2 pos)> BOSS_ENEMIES;

Enemy::Enemy(vec2 pos) {
	sprite.setPosition(pos);
	gui.setTexture(Images::get("enemygui.png"));
	gui.setSize(vec2(ENT_SIZE, ENT_SIZE));
	damageTime = 0.15f;
	spawnTime = 2.0f;
	pathClock = pathTime = 1.0f;
	spawning = true;
	knockSpeed = 300.0f;
}

Enemy::~Enemy() {
	if (gun && (rand() % 10 == 1)) {
		gun->drop();
	} else {
		if (rand() % 5 == 1) {
			game->room->addItem(new PAmmo(sprite.getPosition()));
		} 
		if (rand() % 5 == 1) {
			game->room->addItem(new PHealth(sprite.getPosition()));
		}
		delete gun;
	}
}

void Enemy::update() {
	pathClock += frameTime;
	if (pathClock >= pathTime) {	
		pathfind();
		pathClock = 0;
		pathTime = randFloat(0.8f, 1.2f);
	}

	if (spawning) {
		spawnClock += frameTime;
		if (spawnClock >= spawnTime) {
			pathfind();
			spawning = false;
		}
	} else {
		float x = 0, y = 0, dx = 0, dy = 0;
		Player *nearest = game->nearestPlayer(this);

		float angle = this->angle(*nearest);
		float curx = sprite.getPosition().x;
		float cury = sprite.getPosition().y;
		float width = sprite.getSize().x;
		float height = sprite.getSize().y;

		if (knocking) {
			x = knockSpeed * frameTime * cos(knockAngle);
			y = knockSpeed * frameTime * sin(knockAngle);
		} else if (path.size() > 0) {
			dx = path[0].x * BLOCK_SIZE - curx;
			dy = path[0].y * BLOCK_SIZE - cury;
			float moveAngle = atan2(dy, dx);
			x = speed * frameTime * cos(moveAngle);
			y = speed * frameTime * sin(moveAngle);

			if (abs(dx) < speed * frameTime + 1 && abs(dy) < speed * frameTime + 1) {
				path.erase(path.begin());
			}
		} else {
			x = speed * frameTime * cos(angle);
			y = speed * frameTime * sin(angle);
		}

		int leftCollision = game->collision(this, vec2(curx + x, cury)) || game->collision(this, vec2(curx + x, cury + height));
		int rightCollision = game->collision(this, vec2(curx + width + x, cury)) || game->collision(this, vec2(curx + width + x, cury + height));
		int topCollision = game->collision(this, vec2(curx, cury + y)) || game->collision(this, vec2(curx + width, cury + y));
		int bottomCollision = game->collision(this, vec2(curx, cury + height + y)) || game->collision(this, vec2(curx + width, cury + height + y));

		if (x < 0 && leftCollision) x = 0;
		if (x > 0 && rightCollision) x = 0;
		if (y < 0 && topCollision) y = 0;
		if (y > 0 && bottomCollision) y = 0;

		if (rightCollision || leftCollision) {
			if (dy > 0) { y = speed * frameTime; }
			else if (dy < 0) { y = -speed * frameTime;  }
		}
		if (topCollision || bottomCollision) {
			if (dx > 0) { x = speed * frameTime; }
			else if (dx < 0) { x = -speed * frameTime; }
		}

		leftCollision = game->collision(this, vec2(curx + x, cury)) || game->collision(this, vec2(curx + x, cury + height));
		rightCollision = game->collision(this, vec2(curx + width + x, cury)) || game->collision(this, vec2(curx + width + x, cury + height));
		topCollision = game->collision(this, vec2(curx, cury + y)) || game->collision(this, vec2(curx + width, cury + y));
		bottomCollision = game->collision(this, vec2(curx, cury + height + y)) || game->collision(this, vec2(curx + width, cury + height + y));

		if (x < 0 && leftCollision) x = 0;
		if (x > 0 && rightCollision) x = 0;
		if (y < 0 && topCollision) y = 0;
		if (y > 0 && bottomCollision) y = 0;

		sprite.move(x, y);

		if (gun) {
			if (smart) {
				float bottom = 0.0f, top = 100.0f;
				float mid = (bottom + top) / 2;
				for (int i = 0; i < 25; i++) {
					float playerX = nearest->sprite.getPosition().x;
					float playerY = nearest->sprite.getPosition().y;
					float futurePlayerX = playerX + nearest->speed * mid * cos(nearest->moveAngle);
					float futurePlayerY = playerY + nearest->speed * mid * sin(nearest->moveAngle);
					angle = atan2(futurePlayerY - cury, futurePlayerX - curx);
					float bulletX = curx + gun->speed * mid * cos(angle);
					float bulletY = cury + gun->speed * mid * sin(angle);
					// https://math.stackexchange.com/questions/162728/how-to-determine-if-2-points-are-on-opposite-sides-of-a-line
					float tmp1 = ((futurePlayerY - playerY) * (curx - futurePlayerX)) + ((playerX - futurePlayerX) * (cury - futurePlayerY));
					float tmp2 = ((futurePlayerY - playerY) * (bulletX - futurePlayerX)) + ((playerX - futurePlayerX) * (bulletY - futurePlayerY));
					if (tmp1 * tmp2 < 0) {
						top = mid;
					} else {
						bottom = mid;
					}
					mid = (bottom + top) / 2;
				}
			}
			gun->angle = angle;
			gun->update();
			gun->tryFire();
		}

		if (damaging) {
			damageClock += frameTime;
			if (damageClock >= damageTime) {
				knocking = false;
				damaging = false;
				sprite.setFillColor(sf::Color::White);
			}
		}

		for (int i = 0; i < game->room->bullets.size(); i++) {
			Bullet *cur = game->room->bullets[i];
			if (cur->type == BULLET_PLAYER && cur->intersects(*this)) {
				if (!cur->splash) { 
					knocking = true;
					knockAngle = cur->angle(*this); 
					cur->kill();
				}
				damage(cur->damage);
			}
		}
	}
}

void Enemy::render() {
	if (spawning) {
		sf::RectangleShape spawn;
		spawn.setTexture(Images::get("spawn.png"));
		spawn.setSize(vec2(ENT_SIZE, ENT_SIZE));
		spawn.setOrigin(ENT_SIZE / 2, ENT_SIZE / 2);
		spawn.setScale(spawnClock / spawnTime, spawnClock / spawnTime);
		spawn.setPosition(center());
		window->draw(spawn);
	} else {
		window->draw(sprite);
		if (gun) gun->render();
	}
}

void Enemy::renderMinimap() {
	gui.setPosition(sprite.getPosition());
	window->draw(gui);
}

void Enemy::damage(int amt) {
	if (!spawning) {
		health -= amt;
		damaging = true;
		damageClock = 0;
		sprite.setFillColor(sf::Color::Red);
	}
}

bool Enemy::dead() {
	if (health <= 0) {
		game->room->addParticleSystem(new ParticleSystem(sprite.getPosition(), 100, 10, 0.04f));
		return true;
	}
	return false;
}

void Enemy::pathfind() {
	Player *player = game->nearestPlayer(this);
	game->room->pathfind(path, this, player);
}

EBasic::EBasic(vec2 pos) : Enemy(pos) {
	sprite.setTexture(Images::get("enemy.png"));
	sprite.setSize(vec2(ENT_SIZE * 2 / 3, ENT_SIZE));
	baseSpeed = speed = 100.0f;
	health = maxHealth = 120;
	gun = new GPistol(this);
	gun->fireTime = 1.0f;
}

EAlien::EAlien(vec2 pos) : Enemy(pos) {
	sprite.setTexture(Images::get("alien.png"));
	sprite.setSize(vec2(ENT_SIZE, ENT_SIZE));
	baseSpeed = speed = 75.0f;
	health = maxHealth = 300;
	gun = new GShotgun(this);
	gun->fireTime = 1.5f;
	smart = true;
}

ESlime::ESlime(vec2 pos) : ESlime(pos, 1) { }

ESlime::ESlime(vec2 pos, int level) : Enemy(pos) {
	this->level = level;
	sprite.setTexture(Images::get("slime.png"));
	sprite.setSize(vec2(ENT_SIZE - (level - 1) * 12.0f, ENT_SIZE - (level - 1) * 12.0f));
	baseSpeed = speed = 75.0f + 30.0f * level;
	health = maxHealth = 300 / level;
	hitDamage = 3 - level;
}

ESlime::~ESlime() {
	Enemy::~Enemy();
	if (level < 3) {
		vec2 pos(sprite.getPosition());
		Enemy *baby = game->room->addEnemy(new ESlime(pos, level + 1));
		baby->spawning = false;
		baby = game->room->addEnemy(new ESlime(pos + vec2(32, 0), level + 1));
		baby->spawning = false;
	}
}

Boss::Boss(vec2 pos) : Enemy(pos) { }

void Boss::renderStatic() {
	sf::RectangleShape bar;
	bar.setFillColor(sf::Color::White);
	bar.setSize(vec2(WINDOW_WIDTH / 2, 20));
	bar.setPosition(WINDOW_WIDTH / 4, 10);
	window->draw(bar);

	sf::RectangleShape red;
	bar.setFillColor(sf::Color::Red);
	bar.setSize(vec2((WINDOW_WIDTH / 2 - 4) * ((float)health / (float)maxHealth), 16));
	bar.setPosition(WINDOW_WIDTH / 4 + 2, 12);
	window->draw(bar);
}

EEyeBoss::EEyeBoss(vec2 pos) : Boss(pos) {
	sprite.setSize(vec2(ENT_SIZE * 2, ENT_SIZE * 2));
	sprite.setTexture(Images::get("eye.png"));
	speed = baseSpeed = 50.0f;
	expTime = 5.0f;
	expTotal = 50;
	health = maxHealth = 6000;
	gun = new GBubble(this);
	gun->fireTime = 2.0f;
}

void EEyeBoss::update() {
	Enemy::update();
	if (spawning) return;

	if (exping) {
		if (expCount < expTotal) {
			expCount++;
			game->room->addBullet(new BHeavy(BULLET_ENEMY, center(), 600.0f, expAngle));
			expAngle += 0.5f;
		} else {
			exping = false;
			expAngle = 0;
			expCount = 0;
			expClock = 0;
		}
	} else {
		expClock += frameTime;
		if (expClock >= expTime) {
			exping = true;
		}
	}
}

ERamBoss::ERamBoss(vec2 pos) : Boss(pos) {
	sprite.setSize(vec2(ENT_SIZE * 3, ENT_SIZE * 3));
	sprite.setTexture(Images::get("ram.png"));
	health = maxHealth = 18000;
	chargeMax = 800.0f;
	chargeSpeed = 1200.0f;
	restTime = 2.0f;
	hitDamage = 20;
}

void ERamBoss::update() {
	Enemy::update();
	if (spawning) return;

	Player *player = game->nearestPlayer(this);

	if (charging) {
		float x = chargeSpeed * frameTime * cos(chargeAngle);
		float y = chargeSpeed * frameTime * sin(chargeAngle);
		chargeDist += hypot(y, x);
		sprite.move(x, y);

		if (player->intersects(*this)) {
			chargeDist = chargeMax;
		}

		if (chargeDist >= chargeMax) {
			chargeDist = 0;
			charging = false;
			restClock = 0;
		}
	} else {
		restClock += frameTime;
		if (restClock >= restTime) {
			charging = true;
			chargeAngle = angle(*player);
			Sounds::play("mommy.wav");
		}
	}
}

void ERamBoss::damage(int amt) {
	Enemy::damage(amt);
	if (health <= 0) {
		Sounds::play("mommylove.wav");
	}
}