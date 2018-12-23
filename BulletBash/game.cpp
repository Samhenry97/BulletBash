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
	nextFloor();
	for (Player *player : players) { player->health = player->maxHealth; }
	switchTo(INGAME);
}

void Game::nextFloor() {
	floor++;
	Logger::info("Generating floor " + STR(floor));
	for (int y = 0; y < rooms.size(); y++) {
		for (int x = 0; x < rooms[y].size(); x++) {
			delete rooms[y][x];
		}
	}
	rooms.clear();

	// Generate the floor
	rooms.resize(WORLD_RADIUS * 2 + 1);
	for (int i = 0; i < WORLD_RADIUS * 2 + 1; i++) {
		rooms[i].resize(WORLD_RADIUS * 2 + 1);
	}

	int chests = 2;
	bool boss = false;
	room = new RChest(vec2(WORLD_RADIUS, WORLD_RADIUS));
	rooms[WORLD_RADIUS][WORLD_RADIUS] = room;
	std::queue<GenNode> gen;
	gen.push({ room, vec2(WORLD_RADIUS, WORLD_RADIUS), 0 });
	while (!gen.empty()) {
		GenNode node = gen.front(); gen.pop();
		std::vector<int> available = availableDirs(node.pos);
		if (!available.size()) continue;
		int toadd = available.size() == 1 ? 1 : (rand() % (available.size() - 1)) + 1;
		for (int i = 0; i < toadd; i++) {
			int dir = available[rand() % available.size()];
			vec2 pos = getPos(node.pos, dir);
			Room *next;
			if (rand() % 10 == 1 || (node.level == 3 && chests == 2) || (node.level == 5 && chests == 1)) {
				next = new RChest(pos);
				chests--;
			} else if (node.level == 4 && !boss) {
				next = new RBoss(pos);
				boss = true;
			} else {
				next = new REnemy(pos);
			}
			rooms[pos.y][pos.x] = next;
			node.room->setRoom(next, dir);
			next->setRoom(node.room, (dir + 2) % 4);
			available.erase(find(available.begin(), available.end(), dir));
			if (node.level < MAX_ROOM_LEVEL) {
				gen.push({ next, pos, node.level + 1 });
			}
		}
	}

	for (int i = 0; i < totalPlayers; i++) {
		players[i]->sprite.setPosition(room->center());
	}
	room->start();
	Sounds::back();
}

vec2 Game::getPos(vec2 pos, int dir) {
	switch (dir) {
	case DIR_UP:
		return vec2(pos.x, pos.y - 1);
	case DIR_RIGHT:
		return vec2(pos.x + 1, pos.y);
	case DIR_DOWN:
		return vec2(pos.x, pos.y + 1);
	case DIR_LEFT:
		return vec2(pos.x - 1, pos.y);
	}
	return vec2(0, 0);
}

std::vector<int> Game::availableDirs(vec2 pos) {
	std::vector<int> ret;
	for (int i = 0; i < 4; i++) {
		vec2 newPos = getPos(pos, i);
		if (!rooms[newPos.y][newPos.x]) {
			ret.push_back(i);
		}
	}
	return ret;
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
	case MINIMAP:
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
	case MINIMAP:
		break;
	case PAUSED:
		break;
	case GAMEOVER:
		break;
	}
}

void Game::update() {
	int deadCount = 0;
	float x, y;

	switch (state) {
	case START:
		startMenu->update();
		break;

	case CHARACTER:
		characterMenu->update();
		break;

	case MINIMAP:
		if (!minimapZoomed) {
			x = Xbox::getAxis(minimapController, XBOX_LX) * 0.33f;
			y = Xbox::getAxis(minimapController, XBOX_LY) * 0.33f;
			minimapCenter += vec2(x, y);
			minimap->setCenter(minimapCenter);
			break;
		}

	case INGAME:
		room->update();
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

	case MINIMAP:
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
		if(minimapZoomed) minimap->setCenter(center);
		room->render();
		for (Player* player : players) { 
			player->render();
			addLight(player->sprite.getPosition(), 500);
		}
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
		levelText.setString("Floor " + STR(floor));
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
	if (minimapZoomed) {
		room->renderMinimap();
		for (Player* player : players) { player->renderMinimap(); }
	} else {
		for (int y = 0; y < rooms.size(); y++) {
			for (int x = 0; x < rooms.size(); x++) {
				if (rooms[y][x]) rooms[y][x]->renderMinimap();
			}
		}
		sf::RectangleShape target;
		target.setSize(vec2(100, 100));
		target.setOrigin(vec2(100, 100));
		target.setPosition(minimapCenter);
		target.setTexture(Images::get("target.png"));
		window->draw(target);
	}
}

void Game::transport(int dir) {
	room = room->adj[dir];
	room->start();
	int spawnDir = (dir + 2) % 4;
	vec2 spawn = room->spawnLocation(spawnDir);
	for (int i = 0; i < totalPlayers; i++) {
		players[i]->sprite.setPosition(spawn);
	}
}

void Game::transportToRoom(Room *room) {
	this->room = room;
	vec2 spawn = room->spawnLocation(DIR_LEFT);
	for (int i = 0; i < totalPlayers; i++) {
		players[i]->sprite.setPosition(spawn);
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

void Game::addLight(vec2 pos, int radius) {
	light.setSize(vec2(radius * 2, radius * 2));
	light.setOrigin(vec2(radius, radius));
	light.setPosition(pos - view->getCenter() + vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
	lights->draw(light, sf::BlendAdd);
}

int Game::collision(GameObject *origin, vec2 point) {
	return room->collision(origin, point);
}

int Game::blockCollision(vec2 point) {
	return room->blockCollision(point);
}

void Game::sendButtonPressed(int id, int button) {
	if (state == MINIMAP && id == minimapController) {
		if (button == XBOX_B) {
			minimapZoomed = !minimapZoomed;
		} else if (button == XBOX_A && room->finished) {
			int x = minimapCenter.x / MINIMAP_ROOM_SIZE;
			int y = minimapCenter.y / MINIMAP_ROOM_SIZE;
			if (x >= 0 && y >= 0 && x < rooms[0].size() && y < rooms.size() && rooms[y][x] && rooms[y][x]->visited) {
				transportToRoom(rooms[y][x]);
			}
		}
	} else if (state == INGAME) {
		playerControllers[id]->buttonPressed(button);
	}
}

void Game::sendButtonReleased(int id, int button) {
	if (state == INGAME) {
		playerControllers[id]->buttonReleased(button);
	}
}

void Game::sendAxisMoved(int id, int axis, float pos) {
	if (state == INGAME && axis == XBOX_TRIGGERS && pos > 0.1f) {
		minimapController = id;
		minimapCenter = vec2(room->pos.x * MINIMAP_ROOM_SIZE, room->pos.y * MINIMAP_ROOM_SIZE);
		switchTo(MINIMAP);
	} else if (state == MINIMAP && id == minimapController) {
		if (axis == XBOX_TRIGGERS && pos > -0.1f && pos < 0.1f) {
			switchTo(INGAME);
		}
	}

	if (state == INGAME) {
		playerControllers[id]->axisMoved(axis, pos);
	}
}