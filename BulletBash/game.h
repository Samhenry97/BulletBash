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
	EndGameMenu *endGameMenu;
	int floor;

public:
	enum State { START, CHARACTER, PAUSED, MINIMAP, INGAME, GAMEOVER, END_GAME } state;
	Room *room;
	bool minimapZoomed = true;
	int minimapController;
	vec2 minimapCenter;
	std::vector<Player*> players;
	std::vector<std::vector<Room*>> rooms;
	std::map<int, Player*> playerControllers;

	Game();
	~Game();
	void render();
	void renderStatic();
	void renderMinimap();
	void update();
	void transport(int dir);
	void transportToRoom(Room *room);
	void nextFloor();
	void start();
	void calculateViewPositions();
	void sendButtonPressed(int id, int button);
	void sendButtonReleased(int id, int button);
	void sendAxisMoved(int id, int axis, float pos);
	void switchTo(State newState);
	void addLight(vec2 pos, int radius);
	std::vector<int> availableDirs(vec2 pos);
	vec2 getPos(vec2 pos, int dir);
	int collision(GameObject *origin, vec2 point);
	int blockCollision(vec2 point);
	sf::FloatRect getViewBounds();
	Player *nearestPlayer(GameObject *object);
};

struct GenNode {
	Room *room;
	vec2 pos;
	int level;
};