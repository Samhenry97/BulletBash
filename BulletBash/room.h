#pragma once
#include "const.h"
#include "block.h"
#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "particle.h"

struct Node {
	int y, x;
	float cost;

	bool operator<(const Node &other) const {
		return cost < other.cost;
	}

	bool operator>(const Node &other) const {
		return cost > other.cost;
	}

	bool operator==(const Node &other) const {
		return y == other.y && x == other.x;
	}
};

class Room : GameObject {
private:
	int width, height, waveCount, waveTotal, level;
	std::vector<std::vector<Block*>> blocks;

public:
	std::vector<Pickup*> items;
	std::vector<Enemy*> enemies;
	std::vector<Bullet*> bullets;
	std::vector<ParticleSystem*> particles;

	Room(int width, int height, int level);
	~Room();
	void update();
	void render();
	void renderStatic();
	void renderMinimap();
	void interact(Player *player);
	Pickup *addItem(Pickup *item);
	Bullet *addBullet(Bullet *bullet);
	Enemy *addEnemy(Enemy *enemy);
	ParticleSystem *addParticleSystem(ParticleSystem *system);
	int blockCollision(vec2 point);
	int collision(GameObject *origin, vec2 point);
	bool complete();
	void pathfind(std::vector<vec2i> &path, GameObject *src, GameObject *dest);
	GameObject *nearestEnemy(GameObject *object);
};