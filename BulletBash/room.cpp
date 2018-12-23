#include "room.h"
#include "game.h"
#include "sounds.h"
#include "util.h"
#include "pickup.h"

Room::Room(vec2 pos, std::string roomType) {
	// General setup
	this->roomType = roomType;
	this->pos = pos;
	adj = std::vector<Room*>(4, nullptr);
	
	// Choose and reserve space for the map
	auto roomMap = Maps::loadTemplate(roomType);
	height = roomMap.size() + 2;
	width = roomMap[0].size() + 2;
	blocks.resize(height);
	for (int i = 0; i < height; i++) {
		blocks[i].resize(width);
	}

	// Generate the sides of the room
	for (int i = 0; i < width; i++) {
		blocks[0][i] = new Wall(vec2(i * BLOCK_SIZE, 0));
		blocks[height - 1][i] = new Wall(vec2(i * BLOCK_SIZE, (height - 1) * BLOCK_SIZE));
	}
	for (int i = 1; i < height - 1; i++) {
		blocks[i][0] = new Wall(vec2(0, i * BLOCK_SIZE));
		blocks[i][width - 1] = new Wall(vec2((width - 1) * BLOCK_SIZE, i * BLOCK_SIZE));
	}

	// Now build the map
	for (int y = 1; y < roomMap.size() + 1; y++) {
		for (int x = 1; x < roomMap[y - 1].size() + 1; x++) {
			vec2 pos(x * BLOCK_SIZE, y * BLOCK_SIZE);
			switch (roomMap[y - 1][x - 1]) {
			case 'b':
				blocks[y][x] = new Wall(pos);
				break;
			case 'f':
				blocks[y][x] = new WFire(pos);
				break;
			}
		}
	}

	// Now generate the sprite
	std::string image = "room/back" + STR((rand() % MAX_ROOM_BACK) + 1) + ".png";
	Images::get(image)->setRepeated(true);
	sprite.setTexture(Images::get(image));
	sprite.setTextureRect(sf::IntRect(0, 0, width * BLOCK_SIZE, height * BLOCK_SIZE));
	sprite.setSize(vec2(width * BLOCK_SIZE, height * BLOCK_SIZE));
}

Room::~Room() {
	for (int y = 0; y < blocks.size(); y++) {
		for (int x = 0; x < blocks[y].size(); x++) {
			if (blocks[y][x]) delete blocks[y][x];
		}
	}
	for (int i = 0; i < items.size(); i++) delete items[i];
	for (int i = 0; i < bullets.size(); i++) delete bullets[i];
	for (int i = 0; i < enemies.size(); i++) delete enemies[i];
	for (int i = 0; i < particles.size(); i++) { delete particles[i]; }
}

void Room::start() {
	visited = true;
}

void Room::update() {
	for (int y = 0; y < blocks.size(); y++) {
		for (int x = 0; x < blocks[y].size(); x++) {
			if (blocks[y][x]) blocks[y][x]->update();
		}
	}

	for (int i = 0; i < enemies.size(); i++) { enemies[i]->update(); }
	for (int i = 0; i < bullets.size(); i++) { bullets[i]->update(); }
	for (int i = 0; i < particles.size(); i++) { particles[i]->update(); }

	// Highlight the pickups that can be interacted with
	std::vector<bool> playerHighlight(totalPlayers, false);
	for (int i = 0; i < items.size(); i++) {
		Pickup *cur = items[i];
		cur->update();
		if (cur->interactable()) {
			for (int i = 0; i < totalPlayers; i++) {
				if (!playerHighlight[i] && game->players[i]->intersects(*cur)) {
					playerHighlight[i] = true;
					cur->highlight();
				}
			}
		}
	}

	// Clear all of the entities that need clearing
	for (int i = 0; i < bullets.size(); i++) {
		Bullet *cur = bullets[i];
		if (cur->dead()) {
			bullets.erase(bullets.begin() + i);
			delete cur; i--;
		}
	}

	for (int i = 0; i < enemies.size(); i++) {
		Enemy *cur = enemies[i];
		if (cur->dead()) {
			enemies.erase(enemies.begin() + i);
			delete cur; i--;
		}
	}

	for (int i = 0; i < particles.size(); i++) {
		ParticleSystem *cur = particles[i];
		if (cur->dead()) {
			particles.erase(particles.begin() + i);
			delete cur; i--;
		}
	}
}

void Room::render() {
	window->draw(sprite);
	for (sf::RectangleShape shape : back) { window->draw(shape); }
	for (ParticleSystem *system : particles) { system->render(); }
	for (int y = 0; y < blocks.size(); y++) {
		for (int x = 0; x < blocks[y].size(); x++) {
			if (blocks[y][x]) blocks[y][x]->render();
		}
	}
	for (GameObject *item : items) { item->render(); }
	for (Bullet* bullet : bullets) { bullet->render(); }
	for (Enemy *enemy : enemies) { enemy->render(); }
}

vec2 Room::center() {
	return vec2(width * BLOCK_SIZE / 2, height * BLOCK_SIZE / 2);
}

void Room::renderStatic() {
	for (Enemy *enemy : enemies) { enemy->renderStatic(); }
	for (Bullet* bullet : bullets) { bullet->renderStatic(); }
	for (GameObject *item : items) { item->renderStatic(); }
}

void Room::renderMinimap() {
	if (game->minimapZoomed) {
		sf::RectangleShape back;
		back.setFillColor(sf::Color(50, 50, 50));
		back.setSize(vec2(width * BLOCK_SIZE, height * BLOCK_SIZE));
		window->draw(back);
		for (int y = 0; y < blocks.size(); y++) {
			for (int x = 0; x < blocks[y].size(); x++) {
				if (blocks[y][x]) blocks[y][x]->renderMinimap();
			}
		}
		for (Enemy *enemy : enemies) { enemy->renderMinimap(); }
		for (GameObject *item : items) { item->renderMinimap(); }
	} else {
		sf::RectangleShape connection;
		connection.setFillColor(sf::Color(50, 50, 50));
		connection.setSize(vec2(MINIMAP_ROOM_PAD, MINIMAP_ROOM_PAD));
		if (adj[DIR_DOWN]) {
			connection.setPosition(pos.x * MINIMAP_ROOM_SIZE + MINIMAP_ROOM_DRAW / 2 - MINIMAP_ROOM_PAD / 2, pos.y * MINIMAP_ROOM_SIZE + MINIMAP_ROOM_DRAW);
			window->draw(connection);
		}
		if (adj[DIR_RIGHT]) {
			connection.setPosition(pos.x * MINIMAP_ROOM_SIZE + MINIMAP_ROOM_DRAW, pos.y * MINIMAP_ROOM_SIZE + MINIMAP_ROOM_DRAW / 2 - MINIMAP_ROOM_PAD / 2);
			window->draw(connection);
		}

		int x = game->minimapCenter.x / MINIMAP_ROOM_SIZE;
		int y = game->minimapCenter.y / MINIMAP_ROOM_SIZE;
		sf::RectangleShape back;
		if (game->room == this) {
			back.setFillColor(sf::Color::Green);
		} else if (visited) {
			back.setFillColor(sf::Color::Blue);
		} else {
			back.setFillColor(sf::Color(50, 50, 50));
		}
		if (x == pos.x && y == pos.y) {
			back.setOutlineColor(sf::Color::White);
			back.setOutlineThickness(4.0f);
		}
		back.setSize(vec2(MINIMAP_ROOM_DRAW, MINIMAP_ROOM_DRAW));
		back.setPosition(pos.x * MINIMAP_ROOM_SIZE, pos.y * MINIMAP_ROOM_SIZE);
		window->draw(back);
	}
}

int Room::blockCollision(vec2 point) {
	int x = point.x / BLOCK_SIZE;
	int y = point.y / BLOCK_SIZE;

	if (y >= 0 && x >= 0 && y < height && x < width && blocks[y][x]) {
		return COLLISION_BLOCK;
	}
	return COLLISION_NONE;
}

int Room::collision(GameObject *origin, vec2 point) {
	if (blockCollision(point)) return COLLISION_BLOCK;
	for (Enemy *enemy : enemies) {
		if (origin != enemy && enemy->contains(point)) {
			return COLLISION_ENEMY;
		}
	}
	for (Player *player : game->players) {
		if (origin != player && player->contains(point)) {
			return COLLISION_PLAYER;
		}
	}
	return COLLISION_NONE;
}

bool Room::complete() {
	return finished;
}

void Room::pathfind(std::vector<vec2i> &path, GameObject *src, GameObject *dest) {
	static int dirs[] = { -1, 0, 0, -1, 0, 1, 1, 0 };
	int sx = src->sprite.getPosition().x / BLOCK_SIZE, sy = src->sprite.getPosition().y / BLOCK_SIZE;
	int dx = dest->sprite.getPosition().x / BLOCK_SIZE, dy = dest->sprite.getPosition().y / BLOCK_SIZE;
	if (sx < 0 || sx >= width || sy < 0 || sy >= height) return;
	if (dx < 0 || dx >= width || dy < 0 || dy >= height) return;

	Node start = { sy, sx, hypot(abs(dx - sx), abs(dy - sy)) };
	Node end = { dy, dx, 0 };
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> q;
	std::vector<std::vector<float>> dist(height, std::vector<float>(width, INF));
	std::map<pi, pi> children;

	q.push(start);
	dist[start.y][start.x] = 0;
	bool found = false;

	while (!q.empty() && !found) {
		Node u = q.top(); q.pop();

		for (int i = 0; i < 4; i++) {
			int ny = u.y + dirs[i * 2], nx = u.x + dirs[i * 2 + 1];
			if (nx >= 0 && nx < width && ny >= 0 && ny < height && !blocks[ny][nx]) {
				Node v = { ny, nx, hypot(abs(nx - dx), abs(ny - dy)) };
				if (dist[u.y][u.x] + v.cost < dist[v.y][v.x]) {
					dist[v.y][v.x] = dist[u.y][u.x] + v.cost;
					children[{ v.y, v.x }] = { u.y, u.x };
					if (v == end) { found = true; break; }
					q.push({ ny, nx, dist[v.y][v.x] });
				}
			}
		}
	}
	
	path.clear();
	if (dist[end.y][end.x] != INF) {
		pi cur(end.y, end.x);
		pi last(start.y, start.x);
		while (cur != last) {
			path.push_back(vec2i(cur.second, cur.first));
			cur = children[cur];
		}

		for (int i = 0; i < path.size() / 2; i++) {
			std::swap(path[i], path[path.size() - i - 1]);
		}
	}
}

void Room::interact(Player *player) {
	for (int i = 0; i < items.size(); i++) {
		Pickup *cur = items[i];
		if (cur->interactable() && player->intersects(*cur)) {
			if (cur->interact(player)) {
				items.erase(items.begin() + i);
				i--;
			}
			if (cur->dead()) { delete cur; }
			break;
		}
	}
}

Pickup *Room::addItem(Pickup *item) {
	items.push_back(item);
	return item;
}

Bullet *Room::addBullet(Bullet *bullet) {
	bullets.push_back(bullet);
	return bullet;
}

Enemy *Room::addEnemy(Enemy *enemy) {
	enemies.push_back(enemy);
	return enemy;
}

ParticleSystem *Room::addParticleSystem(ParticleSystem *system) {
	particles.push_back(system);
	return system;
}

GameObject *Room::nearestEnemy(GameObject *object) {
	if (!enemies.size()) return nullptr;
	GameObject *ret = enemies[0];
	float minDist = ret->dist(*object);

	for (int i = 1; i < enemies.size(); i++) {
		float dist = enemies[i]->dist(*object);
		if (dist < minDist) {
			ret = enemies[i];
			minDist = dist;
		}
	}

	return ret;
}

vec2 Room::spawnLocation(int dir) {
	vec2 pos;
	switch (dir) {
	case DIR_UP:
		pos.x = ((width - 1) * BLOCK_SIZE) / 2;
		pos.y = BLOCK_SIZE;
		break;
	case DIR_RIGHT:
		pos.x = (width - 2) * BLOCK_SIZE;
		pos.y = ((height - 1) * BLOCK_SIZE) / 2;
		break;
	case DIR_DOWN:
		pos.x = ((width - 1) * BLOCK_SIZE) / 2;
		pos.y = (height - 2) * BLOCK_SIZE;
		break;
	case DIR_LEFT:
		pos.x = BLOCK_SIZE;
		pos.y = ((height - 1) * BLOCK_SIZE) / 2;
		break;
	}
	return pos;
}

void Room::setRoom(Room *nextRoom, int dir) {
	adj[dir] = nextRoom;
	vec2 pos = spawnLocation(dir);
	addItem(new PDoor(pos, dir));
}

void Room::finish() {
	if (!finished) {
		finished = true;
		for (int i = 0; i < items.size(); i++) {
			if (PDoor *door = dynamic_cast<PDoor*>(items[i])) {
				door->enable();
			}
		}
	}
}

REnemy::REnemy(vec2 pos) : Room(pos, "enemy") {
	waveTotal = rand() % 3 + 1;
}

void REnemy::update() {
	Room::update();
	if (enemies.empty()) {
		if (waveCount <= waveTotal) {
			waveCount++;
			int count = (rand() % 5) + 3;
			for (int i = 0; i < count; i++) {
				float x = rand() % (width - 2) + 1;
				float y = rand() % (height - 2) + 1;
				enemies.push_back(NORMAL_ENEMIES[rand() % NORMAL_ENEMIES.size()](vec2(x * BLOCK_SIZE, y * BLOCK_SIZE)));
			}
		} else {
			finish();
		}
	}
}

RBoss::RBoss(vec2 pos) : Room(pos, "boss") {
	float x = rand() % (width - 2) + 1;
	float y = rand() % (height - 2) + 1;
	enemies.push_back(BOSS_ENEMIES[rand() % BOSS_ENEMIES.size()](vec2(x * BLOCK_SIZE, y * BLOCK_SIZE)));

	nextFloor = new PNextFloor(center());
	items.push_back(nextFloor);
}

void RBoss::update() {
	Room::update();
	if (enemies.empty()) {
		finish();
	}
}

void RBoss::start() {
	Room::start();
	Sounds::boss();
}

void RBoss::finish() {
	if (!finished) {
		Room::finish();
		nextFloor->enable();
		Sounds::back();
	}
}

void RBoss::renderMinimap() {
	Room::renderMinimap();
	sf::RectangleShape shape;
	shape.setTexture(Images::get("ram.png"));
	shape.setSize(vec2(MINIMAP_ROOM_SIZE / 4, MINIMAP_ROOM_SIZE / 4));
	shape.setPosition(vec2(pos.x * MINIMAP_ROOM_SIZE + 250, pos.y * MINIMAP_ROOM_SIZE + 250));
	window->draw(shape);
}

RChest::RChest(vec2 pos) : Room(pos, "chest") {
	sf::RectangleShape platform;
	platform.setSize(vec2(BLOCK_SIZE * 4, BLOCK_SIZE * 4));
	platform.setOrigin(vec2(BLOCK_SIZE * 2, BLOCK_SIZE * 2));
	platform.setPosition(center());
	platform.setTexture(Images::get("platform.png"));
	back.push_back(platform);
	PChest *chest = new PChest(center(), 5);
	chest->sprite.move(-BLOCK_SIZE / 2, -BLOCK_SIZE / 2);
	addItem(chest);
}

void RChest::update() {
	Room::update();
	finish();
}

void RChest::renderMinimap() {
	Room::renderMinimap();
	sf::RectangleShape shape;
	shape.setTexture(Images::get("chest/chest1.png"));
	shape.setSize(vec2(MINIMAP_ROOM_SIZE / 4, MINIMAP_ROOM_SIZE / 4));
	shape.setPosition(vec2(pos.x * MINIMAP_ROOM_SIZE + 250, pos.y * MINIMAP_ROOM_SIZE + 250));
	window->draw(shape);
}