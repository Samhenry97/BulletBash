#pragma once

#include "const.h"

namespace Physics {
	bool intersects(sf::FloatRect &first, sf::FloatRect &second);
	bool lineIntersects(vec2 p1, vec2 q1, vec2 p2, vec2 q2);
}