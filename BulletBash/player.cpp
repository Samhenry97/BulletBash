#include "player.h"
#include "game.h"
#include "bullet.h"
#include "sounds.h"
#include "gun.h"
#include "fonts.h"
#include "util.h"

Player::Player(vec2 pos, vec2 size, std::string image) : GameObject(pos, size) {
	sprite.setTexture(Images::get(image + "1.png"));
	sprite.setPosition(pos);
	sprite.setSize(size);
	gui.setTexture(Images::get(image + "face.png"));
	gui.setSize(vec2(ENT_SIZE, ENT_SIZE));
	reloadOutline.setSize(vec2(BLOCK_SIZE + 4, 16));
	reloadOutline.setFillColor(sf::Color::White);
	reloadProgress.setSize(vec2(BLOCK_SIZE, 12));
	reloadProgress.setFillColor(sf::Color(0, 0, 100));
	controller = 0;
	baseSpeed = speed = 300.0f;
	health = maxHealth = 500;
	dashTime = 0.4f;
	damageTime = 0.5f;
	animTime = 0.2f;
	dropTime = 1.0f;
	animFrame = 1;
	dir = 1;
	guns.push_back(new GPistol(this));
	guns.push_back(new GShotgun(this));
	guns.push_back(new GHoming(this));
	guns.push_back(new GFlameThrower(this));
	guns.push_back(new GMinigun(this));
	guns.push_back(new GGunGun(this));
	guns.push_back(new GSteveGun(this));
	guns.push_back(new GBubble(this));
	gun = guns[0];
	this->image = image;
}

void Player::buttonPressed(int button) {
	switch (button) {
	case XBOX_A:
		game->room->interact(this);
		break;
	case XBOX_Y:
		if(gun) switchGun(false);
		break;
	case XBOX_X:
		if(gun) gun->reload();
		break;
	case XBOX_LB:
		if (dashClock >= dashTime) {
			dashing = true;
			dashClock = 0;
			speed = 500.0f;
			sprite.setTexture(Images::get(image + "roll.png"));
		}
		break;
	case XBOX_START:
		game->switchTo(Game::PAUSED);
		break;
	}
}

void Player::buttonReleased(int button) {

}

void Player::axisMoved(int axis, int pos) {

}

void Player::update() {
	float x, y;
	if (!dashing) {
		float cx = Xbox::getAxis(controller, XBOX_LX);
		float cy = Xbox::getAxis(controller, XBOX_LY);
		speed = hypot(cx, cy) * 2.0f;
		moveAngle = dashAngle = atan2(cy, cx);
		x = speed * frameTime * cos(moveAngle);
		y = speed * frameTime * sin(moveAngle);
	} else {
		x = speed * frameTime * cos(dashAngle);
		y = speed * frameTime * sin(dashAngle);
	}

	sf::FloatRect port = game->getViewBounds();
	float curx = sprite.getPosition().x;
	float cury = sprite.getPosition().y;
	float width = sprite.getSize().x;
	float height = sprite.getSize().y;
	if (x < 0 && curx - x < port.left) x = 0;
	if (x > 0 && curx + x + width > port.left + port.width) x = 0;
	if (y < 0 && cury - y < port.top) y = 0;
	if (y > 0 && cury + y + height > port.top + port.height) y = 0;

	if (x < 0 && (game->collision(this, vec2(curx + x, cury)) || game->collision(this, vec2(curx + x, cury + height)))) x = 0;
	if (x > 0 && (game->collision(this, vec2(curx + width + x, cury)) || game->collision(this, vec2(curx + width + x, cury + height)))) x = 0;
	if (y < 0 && (game->collision(this, vec2(curx, cury + y)) || game->collision(this, vec2(curx + width, cury + y)))) y = 0;
	if (y > 0 && (game->collision(this, vec2(curx, cury + height + y)) || game->collision(this, vec2(curx + width, cury + height + y)))) y = 0;
	sprite.move(vec2(x, y));

	float dx = Xbox::getAxis(controller, XBOX_RX);
	float dy = Xbox::getAxis(controller, XBOX_RY);
	float angle = atan2(dy, dx);
	if (dx == 0.0f && dy == 0.0f) angle = dir == 1 ? 0 : 3.14;

	if (dx < 0.0f) {
		flipLeft();
		dir = -1;
	} else if (dx > 0.0f) {
		flipRight();
		dir = 1;
	} else if(x < 0) {
		flipLeft();
		dir = -1;
	} else if (x > 0) {
		flipRight();
		dir = 1;
	}

	if (gun) {
		gun->update();
		gun->angle = angle;
	}

	if (!dashing) {
		if (gun && Xbox::buttonDown(controller, XBOX_RB)) {
			gun->tryFire();
		}

		if (gun && Xbox::getAxis(controller, XBOX_DY) < 0) {
			if (dropping) {
				dropClock += frameTime;
				if (dropClock >= dropTime) {
					gun->drop();
					switchGun(true);
					dropping = false;
				}
			} else {
				dropping = true;
				dropClock = 0;
			}
		} else {
			dropping = false;
		}
	} else if (dashClock >= dashTime) {
		dashing = false;
		speed = baseSpeed;
		sprite.setTexture(Images::get(image + std::to_string(animFrame) + ".png"));
		dashClock = 0;
	}
	
	dashClock += frameTime;

	if (damaging) {
		damageClock += frameTime;
		if (damageClock >= damageTime) {
			damaging = false;
			sprite.setFillColor(sf::Color::White);
		}
	}

	animClock += frameTime;
	if (!dashing && animClock >= animTime) {
		animFrame++;
		if (animFrame > MAX_ANIM) { animFrame = 1; }
		sprite.setTexture(Images::get(image + std::to_string(animFrame) + ".png"));
		animClock = 0;
	}

	for (int i = 0; i < game->room->bullets.size(); i++) {
		Bullet *cur = game->room->bullets[i];
		if (cur->type == BULLET_ENEMY && cur->intersects(*this)) {
			if (!cur->splash) { cur->kill(); }
			damage(cur->damage);
		}
	}
}

void Player::render() {
	window->draw(sprite);
	if (gun) {
		gun->render();
		if (gun->reloading) {
			reloadOutline.setPosition(sprite.getPosition().x - 2, sprite.getPosition().y - 30);
			reloadProgress.setPosition(sprite.getPosition().x, sprite.getPosition().y - 28);
			reloadProgress.setSize(vec2(BLOCK_SIZE * (gun->reloadClock / gun->reloadTime), reloadProgress.getSize().y));
			window->draw(reloadOutline);
			window->draw(reloadProgress);
		}
	}
}

void Player::renderStatic() {
	float x = controller * 300.0f + 10;
	float y = WINDOW_HEIGHT - 150;

	sf::Text text;
	text.setFillColor(sf::Color::White);
	text.setString("Player " + std::to_string(controller + 1));
	text.setFont(Fonts::get("Raleway-Medium.ttf"));
	text.setCharacterSize(24);
	text.setPosition(x, y);
	window->draw(text);

	y += 26;
	text.setString(gun ? gun->name : "No Gun");
	text.move(0, 26);
	window->draw(text);

	y += 26;
	std::stringstream ammoLeft;
	if (!gun) {
		ammoLeft << "Clip: N/A";
	} else if (gun->clip == -1 && gun->ammo == -1) {
		ammoLeft << "Clip: INF";
	} else if (gun->clip == -1) {
		ammoLeft << "Clip: " << gun->activeAmmo << "/INF";
	} else {
		ammoLeft << "Clip: " << gun->activeAmmo << "/" << gun->clip;
	}
	text.setString(ammoLeft.str());
	text.move(0, 26);
	window->draw(text);

	y += 26;
	std::stringstream activeLeft;
	if (!gun) {
		activeLeft << "Ammo: N/A";
	} else if (gun->ammo == -1) {
		activeLeft << "Ammo: INF";
	} else {
		activeLeft << "Ammo: " << gun->ammo;
	}
	text.setString(activeLeft.str());
	text.move(0, 26);
	window->draw(text);

	y += 30;
	sf::RectangleShape bar;
	bar.setFillColor(sf::Color::White);
	bar.setSize(vec2(300, 20));
	bar.setPosition(x, y);
	window->draw(bar);

	sf::RectangleShape red;
	float percent = (float)health / (float)maxHealth;
	bar.setFillColor(sf::Color(255 - (255 * percent), 255 * percent, 0));
	bar.setSize(vec2(296 * percent, 16));
	bar.setPosition(x + 2, y + 2);
	window->draw(bar);
}

void Player::renderMinimap() {
	gui.setPosition(sprite.getPosition());
	window->draw(gui);
}

void Player::setController(int newController) {
	controller = newController;
}

void Player::damage(int amt) {
	if (!damaging && !dashing) {
		health-= amt;
		damaging = true;
		damageClock = 0;
		sprite.setFillColor(sf::Color::Red);
		Sounds::play("damage.wav");
	}
}

void Player::fillAmmo() {
	if (gun) {
		gun->ammo = gun->startingAmmo;
	}
}

void Player::heal(int amt) {
	health = std::min(health + amt, maxHealth);
}

int Player::type() {
	return BULLET_PLAYER;
}

void Player::addItem(GameObject *item) {
	if (Gun *gun = dynamic_cast<Gun*>(item)) {
		guns.push_back(gun);
		gun->setOwner(this);
	}
}

void Player::switchGun(bool remove) {
	int idx = std::find(guns.begin(), guns.end(), gun) - guns.begin();
	if (remove) {
		guns.erase(guns.begin() + idx);
		idx--;
	}
	gun = guns.size() == 0 ? nullptr : guns[(idx + 1) % guns.size()];
}

void Player::changeImage(std::string newImage) {
	image = newImage;
	gui.setTexture(Images::get(image + "face.png"));
}

bool Player::dead() {
	return health <= 0;
}