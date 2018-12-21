#include "menu.h"
#include "xbox.h"
#include "util.h"
#include "sounds.h"
#include "fonts.h"

Menu::Menu() {
	moveTime = 0.5f;
	selected = 0;
}

Menu::~Menu() {

}

void Menu::changeSelected(int prev, int next) {
	buttons[prev]->deselect();
	buttons[next]->select();
	selected = next;
}

void Menu::back() {

}

void Menu::reset() {
	selected = 0;
}

void Menu::update() {
	float y = Xbox::getAxis(XBOX_LY);
	if (y == 0.0f) y = -Xbox::getAxis(XBOX_DY);
	if (y == 0.0f) {
		moveClock = moveTime;
	} else if (moveClock >= moveTime) {
		if (y < 0.0f) {
			changeSelected(selected, (selected - 1) % buttons.size());
		} else {
			changeSelected(selected, (selected + 1) % buttons.size());
		}
		moveClock = 0;
	} else {
		moveClock += frameTime;
	}

	if (Xbox::buttonDown(XBOX_A)) {
		Xbox::clearInputs();
		Sounds::play("select.wav");
		click();
	}

	if (Xbox::buttonDown(XBOX_B)) {
		Xbox::clearInputs();
		Sounds::play("select.wav");
		back();
	}

	for (MenuButton *button : buttons) {
		button->update();
	}
}

void Menu::render() {
	for (MenuButton *button : buttons) {
		button->render();
	}
}

void Menu::renderStatic() {
	for (MenuButton *button : buttons) {
		button->renderStatic();
	}
}

StartMenu::StartMenu() : Menu() {
	background.setTexture(Images::get("title.png"));
	background.setSize(vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
	logo.setTexture(Images::get("logo.png"));
	logo.setSize(vec2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT * (1.0f / 4.0f)));
	logo.setOrigin(logo.getSize().x / 2, logo.getSize().y / 2);
	logo.setPosition(WINDOW_WIDTH / 2, logo.getSize().y);

	startButton = new MenuButton("Start");
	exitButton = new MenuButton("Exit");

	buttons.push_back(startButton);
	buttons.push_back(exitButton);

	int y = WINDOW_HEIGHT / 2;
	for (int i = 0; i < buttons.size(); i++) {
		buttons[i]->sprite.setPosition(WINDOW_WIDTH / 2, y);
		y += 120;
	}

	buttons[0]->select();
}

void StartMenu::renderStatic() {
	window->draw(background);
	window->draw(logo);
	Menu::renderStatic();
}

void StartMenu::click() {
	if (buttons[selected] == startButton) {
		game->switchTo(Game::CHARACTER);
	} else if (buttons[selected] == exitButton) {
		running = false;
	}
}

PauseMenu::PauseMenu() : Menu() {
	background.setFillColor(sf::Color(100, 100, 100, 200));
	background.setSize(vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
	pausedText.setCharacterSize(75);
	pausedText.setFont(Fonts::get("Raleway-Medium.ttf"));
	pausedText.setFillColor(sf::Color::White);
	pausedText.setOutlineColor(sf::Color::Red);
	pausedText.setOutlineThickness(4.0f);
	pausedText.setString("PAUSED");
	pausedText.setOrigin(pausedText.getLocalBounds().width / 2, pausedText.getLocalBounds().height / 2);
	pausedText.setPosition(WINDOW_WIDTH / 2, 120);

	resumeButton = new MenuButton("Resume");
	exitButton = new MenuButton("Main Menu");

	buttons.push_back(resumeButton);
	buttons.push_back(exitButton);

	int y = WINDOW_HEIGHT / 2;
	for (int i = 0; i < buttons.size(); i++) {
		buttons[i]->sprite.setPosition(WINDOW_WIDTH / 2, y);
		y += 120;
	}

	buttons[0]->select();
}

void PauseMenu::renderStatic() {
	window->draw(background);
	window->draw(pausedText);
	Menu::renderStatic();
}

void PauseMenu::click() {
	if (buttons[selected] == resumeButton) {
		game->switchTo(Game::INGAME);
	} else if (buttons[selected] == exitButton) {
		game->switchTo(Game::START);
	}
}

void PauseMenu::back() {
	game->switchTo(Game::INGAME);
}

CharacterMenu::CharacterMenu() {
	float width = WINDOW_WIDTH / 4;
	float x = WINDOW_WIDTH / 2 - width * totalPlayers / 2;
	data.resize(totalPlayers);
	countTime = 0.5f;
	countdown.setFont(Fonts::get("Raleway-Bold.ttf"));
	countdown.setCharacterSize(WINDOW_HEIGHT / 2);
	countdown.setString("3");
	countdown.setFillColor(sf::Color::Red);
	countdown.setOutlineColor(sf::Color::White);
	countdown.setOutlineThickness(6.0f);
	countdown.setOrigin(countdown.getLocalBounds().width / 2, countdown.getLocalBounds().height / 2);
	countdown.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT * (1.0f / 4.0f));
	for (int i = 0; i < totalPlayers; i++) {
		sf::RectangleShape &cur = data[i].cur;
		sf::RectangleShape &next = data[i].next;
		sf::RectangleShape &cover = data[i].cover;
		sf::Text &ready = data[i].ready;
		data[i].selected = i;
		next.setPosition(x, -WINDOW_HEIGHT);
		next.setSize(vec2(width, WINDOW_HEIGHT));
		cur.setPosition(x, 0);
		cur.setSize(vec2(width, WINDOW_HEIGHT));
		cur.setTexture(Images::get("p" + STR(i + 1) + "logo.png"));
		cover.setPosition(x, 0);
		cover.setSize(vec2(width, WINDOW_HEIGHT));
		cover.setFillColor(sf::Color(255, 255, 255, 150));
		ready.setFont(Fonts::get("Sheeping Cats.otf"));
		ready.setCharacterSize(100);
		ready.setString("READY");
		ready.setFillColor(sf::Color::Green);
		ready.setOutlineColor(sf::Color::Black);
		ready.setOutlineThickness(6.0f);
		ready.setOrigin(ready.getLocalBounds().width / 2, ready.getLocalBounds().height / 2);
		ready.setPosition(x + width / 2, WINDOW_HEIGHT / 2);
		x += width;
	}
}

void CharacterMenu::update() {
	int confirmCount = 0;

	for (int i = 0; i < totalPlayers; i++) {
		Data &info = data[i];
		int controller = game->players[i]->controller;
		if (Xbox::buttonDown(controller, XBOX_A)) {
			Xbox::clearInputs(controller);
			info.confirmed = true;
		}

		if (Xbox::buttonDown(controller, XBOX_B)) {
			if (!info.confirmed) { game->switchTo(Game::START); } 
			else { info.confirmed = false; counting = false; }
			Xbox::clearInputs(controller);
		}

		if (info.scrolling) {
			info.cur.move(0, info.dir * 40);
			info.next.move(0, info.dir * 40);
			float y = info.next.getPosition().y;
			if ((info.dir < 0 && y < 0) || (info.dir > 0 && y > 0)) {
				info.scrolling = false;
				info.cur.setPosition(info.cur.getPosition().x, 0);
				info.cur.setTexture(info.next.getTexture());
				info.next.setPosition(info.next.getPosition().x, -WINDOW_HEIGHT);
			}
		} else if (!info.confirmed) {
			float y = Xbox::getAxis(controller, XBOX_LY);
			if (y == 0.0f) y = -Xbox::getAxis(controller, XBOX_DY);
			if (y != 0.0f) {
				if (y < 0.0f) {
					changeCharacter(i, info.selected, ((info.selected - 1) % MAX_CHARACTER + MAX_CHARACTER) % MAX_CHARACTER, 1);
				} else {
					changeCharacter(i, info.selected, (info.selected + 1) % MAX_CHARACTER, -1);
				}
			}
		} else {
			confirmCount++;
		}
	}

	if (confirmCount >= totalPlayers) {
		if (counting) {
			countClock += frameTime;
			if (countClock >= countTime) {
				countClock = 0;
				count--;
				countdown.setString(STR(count));
				if (count == 0) {
					for (int i = 0; i < totalPlayers; i++) {
						game->players[i]->changeImage("p" + STR(data[i].selected + 1));
					}
					game->start();
				}
			}
		} else {
			counting = true;
			count = 3;
			countdown.setString(STR(count));
			countClock = 0;
		}
	}
}

void CharacterMenu::renderStatic() {
	for (int i = 0; i < totalPlayers; i++) {
		window->draw(data[i].cur);
		window->draw(data[i].next);
		if (data[i].confirmed) {
			window->draw(data[i].cover);
			window->draw(data[i].ready);
		}
	}
	if (counting) window->draw(countdown);
}

void CharacterMenu::click() {

}

void CharacterMenu::back() {
	game->switchTo(Game::START);
}

void CharacterMenu::changeCharacter(int player, int cur, int next, int dir) {
	Sounds::play("hover.wav");
	Data &info = data[player];
	info.scrolling = true;
	info.dir = dir;
	info.next.setTexture(Images::get("p" + STR(next + 1) + "logo.png"));
	info.next.setPosition(info.next.getPosition().x, -dir * WINDOW_HEIGHT);
	info.selected = next;
}

void CharacterMenu::reset() {
	Menu::reset();
	counting = false;
	for (int i = 0; i < data.size(); i++) {
		data[i].confirmed = false;
		data[i].scrolling = false;
	}
}

GameOverMenu::GameOverMenu() {
	deadText.setCharacterSize(100);
	deadText.setFont(Fonts::get("Sheeping Cats.otf"));
	deadText.setFillColor(sf::Color::Red);
	deadText.setOutlineColor(sf::Color::White);
	deadText.setOutlineThickness(8.0f);
	deadText.setString("GAME OVER");
	deadText.setOrigin(deadText.getLocalBounds().width / 2, deadText.getLocalBounds().height / 2);
	deadText.setPosition(WINDOW_WIDTH / 2, 200);

	restartButton = new MenuButton("Quick Restart");
	exitButton = new MenuButton("Main Menu");

	buttons.push_back(restartButton);
	buttons.push_back(exitButton);

	int y = WINDOW_HEIGHT / 2;
	for (int i = 0; i < buttons.size(); i++) {
		buttons[i]->sprite.setPosition(WINDOW_WIDTH / 2, y);
		y += 120;
	}

	buttons[0]->select();
}

void GameOverMenu::renderStatic() {
	window->draw(deadText);
	Menu::renderStatic();
}

void GameOverMenu::click() {
	if (buttons[selected] == restartButton) {
		game->start();
	} else if (buttons[selected] == exitButton) {
		game->switchTo(Game::START);
	}
}