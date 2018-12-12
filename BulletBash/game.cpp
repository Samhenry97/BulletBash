#include "game.h"
#include "logger.h"
#include "sounds.h"
#include "util.h"
#include "fonts.h"

Game::Game() {
	for (int i = 0; i < 4; i++) {
		if (sf::Joystick::isConnected(i)) {
			Player *newPlayer = new Player(vec2(BLOCK_SIZE, BLOCK_SIZE), vec2(ENT_SIZE * 3 / 4, ENT_SIZE), "p" + std::to_string(i + 1));
			newPlayer->setController(i);
			players.push_back(newPlayer);
			playerControllers[i] = newPlayer;
			Xbox::setConnected(i, true);
		}
	}
	totalPlayers = players.size();

	startMenu = new StartMenu();
	pauseMenu = new PauseMenu();
	characterMenu = new CharacterMenu();
	gameOverMenu = new GameOverMenu();
	switchTo(START);

	NORMAL_ENEMIES.push_back(&CREATE_ENEMY<EBasic>);
	NORMAL_ENEMIES.push_back(&CREATE_ENEMY<EAlien>);
	NORMAL_ENEMIES.push_back(&CREATE_ENEMY<ESlime>);
	BOSS_ENEMIES.push_back(&CREATE_ENEMY<ERamBoss>);
	BOSS_ENEMIES.push_back(&CREATE_ENEMY<EEyeBoss>);
}

Game::~Game() {
	for (Player* player : players) { delete player; }
	players.clear();
}

void Game::start() {
	for (Player *player : players) { player->health = player->maxHealth; }
	floor = 1; level = 0; 
	nextRoom();
	switchTo(INGAME);
}

void Game::switchTo(State newState) {
	switch (state) {
	case START:
		startMenu->reset();
		break;
	case CHARACTER:
		characterMenu->reset();
		break;
	case INGAME:
		break;
	case PAUSED:
		pauseMenu->reset();
		break;
	case GAMEOVER:
		break;
	}

	state = newState;
	switch (newState) {
	case START:
		Sounds::music("title.wav");
		break;
	case CHARACTER:
		break;
	case INGAME:
		break;
	case PAUSED:
		break;
	case GAMEOVER:
		break;
	}
}

void Game::update() {
	int deadCount = 0;

	switch (state) {
	case START:
		startMenu->update();
		break;

	case CHARACTER:
		characterMenu->update();
		break;

	case INGAME:
		room->update();
		if (room->complete()) nextRoom();
		for (int i = 0; i < players.size(); i++) { 
			players[i]->update(); 
			if (players[i]->dead()) deadCount++;
		}
		if (deadCount == totalPlayers) { switchTo(GAMEOVER); }
		break;

	case PAUSED:
		pauseMenu->update();
		break;

	case GAMEOVER:
		gameOverMenu->update();
		break;
	}
}

void Game::render() {
	std::vector<float> xpos;
	std::vector<float> ypos;
	float pw, ph, vieww, viewh;
	vec2 size, center;

	switch (state) {
	case START:
		startMenu->render();
		break;

	case CHARACTER:
		characterMenu->render();
		break;

	case PAUSED:
		pauseMenu->render();

	case INGAME:
		size = players[0]->sprite.getSize();
		for (Player *player : players) {
			xpos.push_back(player->sprite.getPosition().x);
			ypos.push_back(player->sprite.getPosition().y);
		}
		std::sort(xpos.begin(), xpos.end());
		std::sort(ypos.begin(), ypos.end());
		pw = xpos[xpos.size() - 1] - xpos[0] + size.x;
		ph = ypos[ypos.size() - 1] - ypos[0] + size.y;
		vieww = view->getSize().x;
		viewh = view->getSize().y;
		if (pw > vieww - 10 && vieww < WINDOW_MAX_WIDTH) { view->zoom(1.002); } else if (ph > viewh - 10 && viewh < WINDOW_MAX_HEIGHT) { view->zoom(1.002); } else if (pw < vieww - 10 && vieww > WINDOW_MIN_WIDTH) { view->zoom(0.998); } else if (ph < viewh - 10 && viewh > WINDOW_MIN_HEIGHT) { view->zoom(0.998); }
		center = vec2((xpos[0] + xpos[xpos.size() - 1] + size.x) / 2, (ypos[0] + ypos[ypos.size() - 1] + size.y) / 2);
		view->setCenter(center);
		minimap->setCenter(center);
		room->render();
		for (Player* player : players) { player->render(); }
		break;

	case GAMEOVER:
		gameOverMenu->render();
		break;
	}
}

void Game::renderStatic() {
	sf::Text levelText;

	switch (state) {
	case START:
		startMenu->renderStatic();
		break;

	case CHARACTER:
		characterMenu->renderStatic();
		break;

	case PAUSED:
		pauseMenu->renderStatic();
		break;

	case INGAME:
		room->renderStatic();
		for (Player* player : players) { player->renderStatic(); }
		levelText.setString("Floor " + STR(floor) + " | Room " + STR(level));
		levelText.setFont(Fonts::get("Raleway-Medium.ttf"));
		levelText.setFillColor(sf::Color::White);
		levelText.setCharacterSize(30);
		levelText.setPosition(10, 10);
		levelText.setOutlineColor(sf::Color::Black);
		levelText.setOutlineThickness(2.0f);
		window->draw(levelText);
		break;

	case GAMEOVER:
		gameOverMenu->renderStatic();
		break;
	}
}

void Game::renderMinimap() {
	if (state == INGAME) {
		room->renderMinimap();
		for (Player* player : players) { player->renderMinimap(); }
	}
}

void Game::nextRoom() {
	delete room;
	level++;
	if (level > 5) {
		level = 1;
		floor++;
	}
	room = new Room(rand() % 10 + 20, rand() % 10 + 20, level);
	for (int i = 0; i < players.size(); i++) {
		players[i]->sprite.setPosition(BLOCK_SIZE, BLOCK_SIZE);
	}
}

Player *Game::nearestPlayer(GameObject *object) {
	Player *ret = players[0];
	float minDist = ret->dist(*object);

	for (int i = 1; i < players.size(); i++) {
		float dist = players[i]->dist(*object);
		if (dist < minDist) {
			ret = players[i];
			minDist = dist;
		}
	}

	return ret;
}

sf::FloatRect Game::getViewBounds() {
	vec2 center = view->getCenter();
	vec2 size = view->getSize();
	return sf::FloatRect(center.x - size.x / 2, center.y - size.y / 2, size.x, size.y);
}

int Game::collision(GameObject *origin, vec2 point) {
	return room->collision(origin, point);
}

int Game::blockCollision(vec2 point) {
	return room->blockCollision(point);
}

void Game::sendButtonPressed(int id, int button) {
	if (state == INGAME) {
		playerControllers[id]->buttonPressed(button);
	}
}

void Game::sendButtonReleased(int id, int button) {
	if (state == INGAME) {
		playerControllers[id]->buttonReleased(button);
	}
}

void Game::sendAxisMoved(int id, int axis, float pos) {
	if (state == INGAME) {
		playerControllers[id]->axisMoved(axis, pos);
	}
}