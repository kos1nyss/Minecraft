#include "object.h"


namespace mc {
	void Object::set_coordinates(float x, float y, float z) {
		coordinates.x = x;
		coordinates.y = y;
		coordinates.z = z;
	}

	void Object::move(float dx, float dy, float dz) {
		coordinates.x += dx;
		coordinates.y += dy;
		coordinates.z += dz;
	}

	Coordinates Object::get_coordinates() {
		return coordinates;
	}
}