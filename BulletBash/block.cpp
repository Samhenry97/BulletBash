#include "block.h"
#include "util.h"

Wall::Wall(vec2 pos) {
	sprite.setTexture(Images::get("block.png"));
	sprite.setPosition(pos);
	sprite.setSize(vec2(BLOCK_SIZE, BLOCK_SIZE));
	hitbox.setPosition(pos);
	hitbox.setSize(vec2(BLOCK_SIZE, BLOCK_SIZE));
}

void Wall::update() {
	GameObject::update();
}

void Wall::render() {
	window->draw(sprite);
}

void Wall::renderMinimap() {
	sf::RectangleShape block;
	block.setFillColor(sf::Color::Black);
	block.setPosition(sprite.getPosition());
	block.setSize(sprite.getSize());
	window->draw(block);
}

WFire::WFire(vec2 pos) : Wall(pos) {
	sprite.setTexture(Images::get("fire.png"));
	animation = new Animation(&sprite, vec2(42, 64), 6, 0.8f);
}

void WFire::render() {
	Wall::render();
	game->addLight(sprite.getPosition(), 200);
}