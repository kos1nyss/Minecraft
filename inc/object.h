#ifndef OBJECT_H
#define OBJECT_H

#include <vector>

namespace mc {
	struct Coordinates {
		float x = 0, y = 0, z = 0;
	};

	class Object {
	public:
		Coordinates coordinates;

		void set_coordinates(float x, float y, float z);

		void move(float dx, float dy, float dz);

		Coordinates get_coordinates();
	};
}

#endif