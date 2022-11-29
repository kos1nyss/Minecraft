#ifndef BLOCK_H
#define BLOCK_H

#include "object.h"
#include "player.h"

namespace mc {
	class Block : public Object {
	private:
		bool* neighbors;
		bool was_drawn = false;

		int facets[6][4] = {
			{0, 1, 5, 4},
			{3, 2, 6, 7},
			{0, 1, 2, 3},
			{4, 5, 6, 7},
			{0, 3, 7, 4},
			{1, 2, 6, 5},
		};

		std::vector<int> get_visible_faces(Player& player);

	public:
		int vertex_shifts[24] = {
			0, 0, 0,
			0, 0, 1,
			1, 0, 1,
			1, 0, 0,
			0, 1, 0,
			0, 1, 1,
			1, 1, 1,
			1, 1, 0,
		};

		float color[3];
		std::vector<float> size = { 1, 1, 1 };
		void draw(Player& player);

		void set_neighbors(bool* new_neighbors);

		void set_color(int r, int g, int b);

		void shake_color();

		bool get_was_drawn();

		void allow_drawing();
	};
}
#endif