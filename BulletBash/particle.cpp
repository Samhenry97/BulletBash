#include "particle.h"
#include "util.h"

ParticleSystem::ParticleSystem(vec2 pos, int maxParticles, int perIter, float timeBetween) {
	sprite.setPosition(pos);
	this->maxParticles = maxParticles;
	this->perIter = perIter;
	this->timeBetween = timeBetween;
	curParticles = 0;
}

ParticleSystem::~ParticleSystem() {
	for (int i = 0; i < particles.size(); i++) {
		delete particles[i];
	}
}

void ParticleSystem::update() {
	timeAnim += frameTime;
	if (curParticles < maxParticles && timeAnim >= timeBetween) {
		for (int i = 0; i < perIter; i++) {
			float angle = randFloat(0, 360);
			particles.push_back(new Particle(sprite.getPosition(), 200.0f, angle, 150.0f));
		}
		curParticles += perIter;
		timeAnim = 0.0f;
	}

	for (int i = 0; i < particles.size(); i++) {
		Particle *cur = particles[i];
		if (cur->dead()) {
			particles.erase(particles.begin() + i);
			delete cur; i--;
		} else {
			cur->update();
		}
	}
}

void ParticleSystem::render() {
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->render();
	}
}

bool ParticleSystem::dead() {
	return particles.size() == 0 && curParticles >= maxParticles;
}

Particle::Particle(vec2 pos, float speed, float angle, float maxDist) {
	shape.setPosition(pos);
	int r = randFloat(0, 256);
	int g = randFloat(0, 256);
	int b = randFloat(0, 256);
	shape.setFillColor(sf::Color(r, g, b));
	shape.setRadius(6.0f);
	this->speed = speed;
	this->angle = angle;
	this->maxDist = maxDist;
	dist = 0;
}

void Particle::update() {
	float x = speed * frameTime * cos(angle);
	float y = speed * frameTime * sin(angle);
	dist += hypot(x, y);
	shape.move(x, y);
}

void Particle::render() {
	window->draw(shape);
}

bool Particle::dead() {
	return dist >= maxDist;
}