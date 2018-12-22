#pragma once

#include "const.h"
#include "block.h"
#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "particle.h"
#include "mapmanager.h"

class Room : GameObject {
protected:
	int width, height, level;
	bool finished;
	std::vector<std::vector<Wall*>> blocks;
	std::string roomType = "default";

public:
	std::vector<sf::RectangleShape> back;
	std::vector<Pickup*> items;
	std::vector<Enemy*> enemies;
	std::vector<Bullet*> bullets;
	std::vector<ParticleSystem*> particles;
	std::vector<Room*> adj;

	Room(int level, std::string roomType);
	~Room();
	virtual void update();
	virtual void render();
	virtual void renderStatic();
	virtual void renderMinimap();
	void interact(Player *player);
	Pickup *addItem(Pickup *item);
	Bullet *addBullet(Bullet *bullet);
	Enemy *addEnemy(Enemy *enemy);
	ParticleSystem *addParticleSystem(ParticleSystem *system);
	int blockCollision(vec2 point);
	int collision(GameObject *origin, vec2 point);
	bool complete();
	void setRoom(Room *nextRoom, int dir);
	void finish();
	vec2 spawnLocation(int dir);
	vec2 center();
	std::vector<int> availableDirs();
	void pathfind(std::vector<vec2i> &path, GameObject *src, GameObject *dest);
	GameObject *nearestEnemy(GameObject *object);
};

class REnemy : public Room {
private:
	int waveCount, waveTotal;

public:
	REnemy(int level);
	void update();
};

class RChest : public Room {
public:
	RChest(int level);
	void update();
};

class RBoss : public Room {
public:
	RBoss(int level);
	void update();
};

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

struct GenNode {
	Room *room;
	int level;
};