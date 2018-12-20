#pragma once

#include "const.h"
#include "gameobject.h"

class Particle : public GameObject {
private:
	sf::CircleShape shape;
	float speed;
	float angle;
	float dist;
	float maxDist;

public:
	Particle(vec2 pos, float speed, float angle, float maxDist);
	void update();
	void render();
	bool dead();
};

class ParticleSystem : public GameObject {
private:
	std::vector<Particle*> particles;
	int maxParticles;
	int curParticles;
	int perIter;
	float timeAnim, timeBetween;

public:
	ParticleSystem(vec2 pos, int maxParticles, int perIter, float timeBetween);
	~ParticleSystem();
	void update();
	void render();
	bool dead();
};