#include "gameobject.h"

GameObject::GameObject() {

}

GameObject::GameObject(vec2 pos, vec2 size) {
	setPosition(pos);
	setSize(size);
}

GameObject::~GameObject() {
	delete animation;
}

bool GameObject::intersects(GameObject *other) {
	if (!solid || !other->solid) return false;
	return hitbox.getGlobalBounds().intersects(other->hitbox.getGlobalBounds());
}

bool GameObject::contains(vec2 point) {
	return hitbox.getGlobalBounds().contains(point);
}

void GameObject::setPosition(vec2 newPos) {
	sprite.setPosition(newPos);
	hitbox.setPosition(newPos);
}

void GameObject::setSize(vec2 newSize) {
	sprite.setSize(newSize);
	hitbox.setSize(newSize);
}

void GameObject::setRotation(float newRotation) {
	sprite.setRotation(newRotation);
	hitbox.setRotation(newRotation);
}

void GameObject::setOrigin(vec2 newOrigin) {
	sprite.setOrigin(newOrigin);
	hitbox.setOrigin(newOrigin);
}

void GameObject::move(vec2 offset) {
	sprite.move(offset);
	hitbox.move(offset);
}

float GameObject::dist(GameObject *other) {
	float x = other->hitbox.getPosition().x - hitbox.getPosition().x;
	float y = other->hitbox.getPosition().y - hitbox.getPosition().y;
	return std::hypot(x, y);
}

float GameObject::angle(GameObject *other) {
	float x = other->hitbox.getPosition().x - hitbox.getPosition().x;
	float y = other->hitbox.getPosition().y - hitbox.getPosition().y;
	return atan2(y, x);
}

vec2 GameObject::center() {
	return vec2(hitbox.getPosition().x + hitbox.getSize().x / 2, hitbox.getPosition().y + hitbox.getSize().y / 2);
}

void GameObject::origin() {
	sprite.setOrigin(sprite.getSize().x / 2, sprite.getSize().y / 2);
	hitbox.setOrigin(hitbox.getSize().x / 2, hitbox.getSize().y / 2);
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

std::vector<vec2> GameObject::transformPoints() {
	sf::Transform transform = hitbox.getTransform();
	return {
		transform.transformPoint(vec2(0, 0)),
		transform.transformPoint(vec2(hitbox.getSize().x, 0)),
		transform.transformPoint(vec2(hitbox.getSize().x, hitbox.getSize().y)),
		transform.transformPoint(vec2(0, hitbox.getSize().y)),
	};
}