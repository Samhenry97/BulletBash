#include "room.h"
#include "game.h"
#include "sounds.h"
#include "util.h"

Room::Room(int width, int height, int level) {
	std::string image = "room/back" + STR(rand() % MAX_ROOM_BACK + 1) + ".png";
	Images::get(image)->setRepeated(true);
	sprite.setTexture(Images::get(image));
	sprite.setTextureRect(sf::IntRect(0, 0, width * BLOCK_SIZE, height * BLOCK_SIZE));
	sprite.setSize(vec2(width * BLOCK_SIZE, height * BLOCK_SIZE));

	if (level % 5 == 0) { waveTotal = 1; Sounds::boss(); }
	else { waveTotal = 3; Sounds::back();  }

	this->width = width;
	this->height = height;
	this->level = level;
	blocks.resize(height);
	for (int i = 0; i < height; i++) {
		blocks[i].resize(width);
	}

	for (int i = 0; i < width; i++) {
		blocks[0][i] = new Block(0, i * BLOCK_SIZE);
		blocks[height - 1][i] = new Block((height - 1) * BLOCK_SIZE, i * BLOCK_SIZE);
	}

	for (int i = 1; i < height - 1; i++) {
		blocks[i][0] = new Block(i * BLOCK_SIZE, 0);
		blocks[i][width - 1] = new Block(i * BLOCK_SIZE, (width - 1) * BLOCK_SIZE);
	}

	for (int i = 0; i < 15; i++) {
		int y = (rand() % (height - 2)) + 1;
		int x = (rand() % (width - 2)) + 1;

		blocks[y][x] = new Block(y * BLOCK_SIZE, x * BLOCK_SIZE);
		blocks[y + 1][x] = new Block((y + 1) * BLOCK_SIZE, x * BLOCK_SIZE);
		blocks[y][x + 1] = new Block(y * BLOCK_SIZE, (x + 1) * BLOCK_SIZE);
		blocks[y + 1][x + 1] = new Block((y + 1) * BLOCK_SIZE, (x + 1) * BLOCK_SIZE);
	}
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
}

void Room::update() {
	if (enemies.empty()) {
		if (waveCount <= waveTotal) {
			waveCount++;
			if (level % 5 == 0) {
				float x = rand() % (width - 2) + 1;
				float y = rand() % (height - 2) + 1;
				enemies.push_back(BOSS_ENEMIES[rand() % BOSS_ENEMIES.size()](vec2(x * BLOCK_SIZE, y * BLOCK_SIZE)));
			} else {
				int count = (rand() % 5) + 5;
				for (int i = 0; i < count; i++) {
					float x = rand() % (width - 2) + 1;
					float y = rand() % (height - 2) + 1;
					enemies.push_back(NORMAL_ENEMIES[rand() % NORMAL_ENEMIES.size()](vec2(x * BLOCK_SIZE, y * BLOCK_SIZE)));
				}
			}
		}
	}

	for (int y = 0; y < blocks.size(); y++) {
		for (int x = 0; x < blocks[y].size(); x++) {
			if (blocks[y][x]) blocks[y][x]->update();
		}
	}

	for (int i = 0; i < enemies.size(); i++) { enemies[i]->update(); }
	for (int i = 0; i < bullets.size(); i++) { bullets[i]->update(); }
	for (int i = 0; i < items.size(); i++) { items[i]->update(); }

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
}

void Room::render() {
	window->draw(sprite);
	for (int y = 0; y < blocks.size(); y++) {
		for (int x = 0; x < blocks[y].size(); x++) {
			if (blocks[y][x]) blocks[y][x]->render();
		}
	}
	for (Enemy *enemy : enemies) { enemy->render(); }
	for (Bullet* bullet : bullets) { bullet->render(); }
	for (GameObject *item : items) { item->render(); }
}

void Room::renderStatic() {
	for (Enemy *enemy : enemies) { enemy->renderStatic(); }
	for (Bullet* bullet : bullets) { bullet->renderStatic(); }
	for (GameObject *item : items) { item->renderStatic(); }
}

void Room::renderMinimap() {
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
	return waveCount == waveTotal && enemies.empty();
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
		if (player->intersects(*items[i])) {
			player->addItem(items[i]);
			items.erase(items.begin() + i);
			i--;
		}
	}
}

GameObject *Room::addItem(GameObject *item) {
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