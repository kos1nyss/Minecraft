#include <iostream>

#define _USE_MATH_DEFINES

#include <math.h>

#include "ray.h"
#include "block.h"

namespace mc {
	void Ray::cast(World& world, float length,
		float x, float y, float z,
		float ax, float ay) {
		history.clear();
		ax = ax / 180 * M_PI;
		ay = ay / 180 * M_PI - M_PI_2;
		float unit_vector_x = cos(ay) * cos(ax), unit_vector_y = sin(ax), unit_vector_z = sin(ay) * cos(ax);
		for (int i = 0; i < (length * accuracy); i++) {
			x += unit_vector_x / accuracy;
			y += unit_vector_y / accuracy;
			z += unit_vector_z / accuracy;
			Coordinates c;
			c.x = floor(x); c.y = floor(y); c.z = floor(z);

			if (world.within_the_world(c.x, c.y, c.z)) {
				history.push_back(c);
				Block* block = world.get_block(x, y, z);
				if (block) {
					break;
				}
			}
			else {
				break;
			}
		}
	}

	std::vector<Coordinates> Ray::get_history() {
		return history;
	}
}