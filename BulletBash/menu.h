#pragma once

#include "menubutton.h"

class Menu {
protected:
	int selected;
	float moveClock, moveTime;
	std::vector<MenuButton*> buttons;

public:
	Menu();
	~Menu();
	void changeSelected(int prev, int next);
	virtual void click() = 0;
	virtual void back();
	virtual void update();
	virtual void render();
	virtual void renderStatic();
	virtual void reset();
};

class StartMenu : public Menu {
	MenuButton *startButton;
	MenuButton *exitButton;

	sf::RectangleShape background;
	sf::RectangleShape logo;

public:
	StartMenu();
	void renderStatic();
	void click();
};

class PauseMenu : public Menu {
	MenuButton *resumeButton;
	MenuButton *exitButton;
	
	sf::Text pausedText;
	sf::RectangleShape background;

public:
	PauseMenu();
	void renderStatic();
	void click();
	void back();
};

class CharacterMenu : public Menu {
	struct Data {
		sf::RectangleShape cur;
		sf::RectangleShape next;
		sf::RectangleShape cover;
		sf::Text ready;
		bool confirmed, scrolling;
		int selected, dir;
	};

	std::vector<Data> data;
	float countClock, countTime;
	bool counting;
	int count;
	sf::Text countdown;

public:
	CharacterMenu();
	void renderStatic();
	void update();
	void click();
	void back();
	void reset();
	void changeCharacter(int player, int cur, int next, int dir);
};

class GameOverMenu : public Menu {
private:
	MenuButton *restartButton;
	MenuButton *exitButton;

	sf::Text deadText;

public:
	GameOverMenu();
	void renderStatic();
	void click();
};

class EndGameMenu : public Menu {
private:
	MenuButton *restartButton;
	MenuButton *mainButton;

	sf::Text winText;

public:
	EndGameMenu();
	void renderStatic();
	void click();
};