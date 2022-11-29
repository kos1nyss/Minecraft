#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "block.h"
#include "player.h"

namespace mc {
	class World {
	public:
		int visibility_range = 50;
		int size_x = 500, size_y = 50, size_z = 500;
		Block* blocks[500][65][500];

		void generate();

		void draw(Player& player);

		bool within_the_world(float x, float y, float z);

		Block* get_block(float x, float y, float z);

		void new_block(float x, float y, float z);

		void update_neighbors(int x, int y, int z, bool main_block);

		void delete_block(int x, int y, int z);
	};
}

#endif