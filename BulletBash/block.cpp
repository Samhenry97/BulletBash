#include "block.h"
#include "util.h"

Block::Block(float y, float x) {
	sprite.setTexture(Images::get("block.png"));
	sprite.setPosition(x, y);
	sprite.setSize(vec2(BLOCK_SIZE, BLOCK_SIZE));
}

void Block::update() {

}

void Block::render() {
	window->draw(sprite);
}

void Block::renderMinimap() {
	sf::RectangleShape block;
	block.setFillColor(sf::Color::Black);
	block.setPosition(sprite.getPosition());
	block.setSize(sprite.getSize());
	window->draw(block);
}