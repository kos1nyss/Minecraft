#include "player.h"
#include "ray.h"


namespace mc {
	void Player::set_angle(float x, float y) {
		angle.x = x;
		angle.y = y;
	}

	Angle Player::get_angle() {
		return angle;
	}

	MoveY Player::get_move_y() {
		return move_y;
	}

	void Player::update_y(float delta_time) {
		move_y.speed += move_y.a * delta_time;
	}

	void Player::set_y_speed(float new_speed) {
		move_y.speed = new_speed;
	}
}