#include "physics.h"
#include "util.h"

namespace Physics {
	bool intersects(sf::FloatRect &first, sf::FloatRect &second) {
		return first.intersects(second);
	}

	bool lineIntersects(vec2 p1, vec2 q1, vec2 p2, vec2 q2) {
		vec2 s1, s2;
		s1.x = q1.x - p1.x; 
		s1.y = q1.y - p1.y;
		s2.x = q2.x - p2.x;
		s2.y = q2.y - p2.y;

		float d = -s2.x * s1.y + s1.x * s2.y;
		float s = (-s1.y * (p1.x - p2.x) + s1.x * (p1.y - p2.y)) / d;
		float t = (s2.x * (p1.y - p2.y) - s2.y * (p1.x - p2.x)) / d;

		return s >= 0 && s <= 1 && t >= 0 && t <= 1;
	}
}