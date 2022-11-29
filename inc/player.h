#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"
#include "player.h"

namespace mc {
	struct Angle {
		float x = 0, y = 0;
	};

	struct MoveY {
		float a = -20, speed = 0;
	};

	class Player : public Object {
	public:
		Angle angle;
		MoveY move_y;

		void set_angle(float x, float y);

		Angle get_angle();

		MoveY get_move_y();

		void update_y(float delta_time);

		void set_y_speed(float new_speed);
	};
}

#endif