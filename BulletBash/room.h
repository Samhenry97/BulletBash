#pragma once

#include "const.h"
#include "block.h"
#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "particle.h"
#include "mapmanager.h"

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
protected:
	std::vector<std::vector<Wall*>> blocks;
	std::string roomType = "default";
	int width, height;

public:
	std::vector<sf::RectangleShape> back;
	std::vector<Pickup*> items;
	std::vector<Enemy*> enemies;
	std::vector<Bullet*> bullets;
	std::vector<ParticleSystem*> particles;
	std::vector<Room*> adj;
	vec2 pos;
	bool visited, finished;

	Room(vec2 pos, std::string roomType);
	~Room();
	virtual void start();
	virtual void update();
	virtual void render();
	virtual void renderStatic();
	virtual void renderMinimap();
	virtual void finish();
	void interact(Player *player);
	Pickup *addItem(Pickup *item);
	Bullet *addBullet(Bullet *bullet);
	Enemy *addEnemy(Enemy *enemy);
	ParticleSystem *addParticleSystem(ParticleSystem *system);
	int blockCollision(vec2 point);
	int collision(GameObject *origin, vec2 point);
	float raycastLength();
	RayCastResult raycast(vec2 pos, float angle, float len);
	void genline(std::vector<vec2> &line, vec2 pos, vec2 endPos);
	bool complete();
	void setRoom(Room *nextRoom, int dir);
	vec2 spawnLocation(int dir);
	vec2 center();
	void pathfind(std::vector<vec2i> &path, GameObject *src, GameObject *dest);
	GameObject *nearestEnemy(GameObject *object);
};

class REnemy : public Room {
private:
	int waveCount, waveTotal;

public:
	REnemy(vec2 pos);
	void update();
};

class RChest : public Room {
public:
	RChest(vec2 pos);
	void update();
	void renderMinimap();
};

class RBoss : public Room {
private:
	PNextFloor *nextFloor;

public:
	RBoss(vec2 pos);
	void update();
	void start();
	void finish();
	void renderMinimap();
};