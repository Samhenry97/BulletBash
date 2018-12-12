#include "menubutton.h"
#include "fonts.h"
#include "util.h"
#include "sounds.h"

MenuButton::MenuButton(std::string content) {
	sprite.setTexture(Images::get("button.png"));
	sprite.setSize(vec2(300.0f, 80.0f));
	sprite.setFillColor(sf::Color(100, 100, 100, 200));
	text.setFont(Fonts::get("Raleway-Medium.ttf"));
	text.setCharacterSize(40);
	text.setString(content);
	origin();
	this->content = content;
}

void MenuButton::select() {
	selected = true;
	Sounds::play("hover.wav");
	text.setFillColor(sf::Color::White);
	sprite.setFillColor(sf::Color::White);
	flipUp();
}

void MenuButton::deselect() {
	selected = false;
	text.setFillColor(sf::Color(100, 100, 100, 200));
	sprite.setFillColor(sf::Color(100, 100, 100, 200));
	flipDown();
}

void MenuButton::update() {

}

void MenuButton::render() {

}

void MenuButton::renderStatic() {
	text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
	text.setPosition(sprite.getPosition() - vec2(0, 15));

	window->draw(sprite);
	window->draw(text);
}