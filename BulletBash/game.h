#pragma once

#include "const.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "room.h"
#include "menu.h"

class Game {
private:
	StartMenu *startMenu;
	PauseMenu *pauseMenu;
	CharacterMenu *characterMenu;
	GameOverMenu *gameOverMenu;
	int floor, level;

public:
	enum State { START, CHARACTER, PAUSED, INGAME, GAMEOVER } state;
	Room *room;
	std::vector<Player*> players;
	std::map<int, Player*> playerControllers;

	Game();
	~Game();
	void render();
	void renderStatic();
	void renderMinimap();
	void update();
	void nextRoom();
	void start();
	void sendButtonPressed(int id, int button);
	void sendButtonReleased(int id, int button);
	void sendAxisMoved(int id, int axis, float pos);
	void switchTo(State newState);
	int collision(GameObject *origin, vec2 point);
	int blockCollision(vec2 point);
	sf::FloatRect getViewBounds();
	Player *nearestPlayer(GameObject *object);
};