#ifndef RAY_H
#define RAY_H

#include "world.h"
#include <vector>
#include "object.h"

namespace mc {
	class Ray {
	public:
		void cast(World& world, float length,
			float x, float y, float z,
			float ax, float ay);

		std::vector<Coordinates> get_history();

	private:
		std::vector<Coordinates> history;
		float accuracy = 10.f;
	};

}

#endif 
