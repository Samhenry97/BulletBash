#include "gameobject.h"

GameObject::GameObject() {

}

GameObject::GameObject(vec2 pos, vec2 size) {
	sprite.setPosition(pos);
	sprite.setSize(size);
}

GameObject::~GameObject() {
	delete animation;
}

bool GameObject::intersects(GameObject &other) {
	return sprite.getGlobalBounds().intersects(other.sprite.getGlobalBounds());
}

bool GameObject::contains(vec2 point) {
	return sprite.getGlobalBounds().contains(point);
}

float GameObject::dist(GameObject &other) {
	float x = std::abs(other.sprite.getPosition().x - sprite.getPosition().x);
	float y = std::abs(other.sprite.getPosition().y - sprite.getPosition().y);
	return std::hypot(x, y);
}

float GameObject::angle(GameObject &other) {
	float x = other.sprite.getPosition().x - sprite.getPosition().x;
	float y = other.sprite.getPosition().y - sprite.getPosition().y;
	return atan2(y, x);
}

vec2 GameObject::center() {
	return vec2(sprite.getPosition().x + sprite.getSize().x / 2, sprite.getPosition().y + sprite.getSize().y * 0.75f);
}

void GameObject::origin() {
	sprite.setOrigin(sprite.getSize().x / 2, sprite.getSize().y / 2);
}

int GameObject::type() {
	return BULLET_ENEMY;
}

void GameObject::update() {
	if (animation) {
		animation->update();
	}
}

void GameObject::renderStatic() {

}

void GameObject::renderMinimap() {

}

void GameObject::flipLeft() {
	sf::Vector2u size = sprite.getTexture()->getSize();
	sprite.setTextureRect(sf::IntRect(size.x, 0, -(int) size.x, size.y));
}

void GameObject::flipRight() {
	sf::Vector2u size = sprite.getTexture()->getSize();
	sprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y));
}

void GameObject::flipUp() {
	sf::Vector2u size = sprite.getTexture()->getSize();
	sprite.setTextureRect(sf::IntRect(0, size.y, size.x, -(int) size.y));
}

void GameObject::flipDown() {
	sf::Vector2u size = sprite.getTexture()->getSize();
	sprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y));
}