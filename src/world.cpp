#include "world.h"
#include "player.h"
#include "perlin_noise.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <iostream>
#include <GL/glut.h>
#include <map>

namespace mc {
	void World::generate() {
		PerlinNoise noise;
		float** noise_matrix = noise.create(1000);

		for (float x = 0; x < size_x; x++) {
			for (float z = 0; z < size_z; z++) {
				Block* new_block = new Block();
				float new_y = noise_matrix[(int)x][(int)z] * size_y;
				new_block->set_coordinates(x, floor(new_y), z);
				if (new_y / size_y > 0.8) {
					new_block->set_color(255, 255, 255);
				}
				else if (new_y / size_y > 0.7) {
					new_block->set_color(180, 180, 180);
				}
				else {
					new_block->set_color(100, 240, 100);
				}
				new_block->shake_color();
				blocks[(int)x][(int)new_y][(int)z] = new_block;
			}
		}

		for (int x = 0; x < size_x; x++) {
			for (int y = 0; y < size_y; y++) {
				for (int z = 0; z < size_z; z++) {
					update_neighbors(x, y, z, false);
				}
			}
		}


		for (int x = 0; x < 1000; x++) {
			delete[] noise_matrix[x];
		}
		delete[] noise_matrix;
	}

	void World::draw(Player& player) {
		Block block;

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_INT, 0, &(block.vertex_shifts));

		float angle = player.angle.y / 180 * M_PI - M_PI_2;
		float unit_vector_x = cos(angle);
		float unit_vector_z = sin(angle);
		int x, z, vr = visibility_range, dx, dz;
		std::vector<Block*> drawn_blocks;
		Coordinates player_coord = player.get_coordinates();
		int count = 0;
		for (float i = -3; i < 60; i += 0.4f) {
			for (int j = -25; j < 25; j++) {
				x = player_coord.x + i * unit_vector_x + j * unit_vector_z;
				z = player_coord.z + i * unit_vector_z - j * unit_vector_x;
				dx = x - int(player_coord.x);
				dz = z - int(player_coord.z);
				if (dx < 0) {
					dx = 150 + dx;
				}
				if (dz < 0) {
					dz = 150 + dz;
				}
				if (within_the_world(x, 0, z)) {
					for (int y = 0; y < size_y; y++) {
						if (blocks[x][y][z] && !(blocks[x][y][z]->get_was_drawn())) {
							blocks[x][y][z]->draw(player);
							count++;
							drawn_blocks.push_back(blocks[x][y][z]);
						}
					}
				}
			}
		}
		for (int i = 0; i < count; i++) {
			drawn_blocks[i]->allow_drawing();
		}
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	bool World::within_the_world(float x, float y, float z) {
		return (x >= 0) && (x < size_x) && (z >= 0) && (z < size_z) && (y >= 0) && (y < size_y);
	}

	Block* World::get_block(float x, float y, float z) {
		return blocks[(int)x][(int)y][(int)z];
	}

	void World::update_neighbors(int x, int y, int z, bool main_block) {
		int shifts[2] = { 1, -1 };
		bool* neighbors = new bool[6];
		for (int n = 0; n < 3; n++) {
			for (int s = 0; s < 2; s++) {
				int c[3] = { x, y, z };
				c[n] -= shifts[s];
				if (within_the_world(c[0], c[1], c[2])) {
					if (blocks[c[0]][c[1]][c[2]]) {
						neighbors[n * 2 + s] = true;
						if (main_block) {
							update_neighbors(c[0], c[1], c[2], false);
						}
					}
					else {
						neighbors[n * 2 + s] = false;
					}
				}
				else {
					neighbors[n * 2 + s] = true;
				}
			}
		}

		if (blocks[x][y][z]) {
			blocks[x][y][z]->set_neighbors(neighbors);
		}
		else {
			delete[] neighbors;
		}
	}

	void World::new_block(float x, float y, float z) {
		Block* block = new Block();
		block->set_coordinates(x, y, z);
		block->set_color(100 + rand() % 100, 100 + rand() % 100, 100 + rand() % 100);
		blocks[(int)x][(int)y][(int)z] = block;
		update_neighbors((int)x, (int)y, (int)z, true);
	}


	void World::delete_block(int x, int y, int z) {
		delete blocks[x][y][z];
		blocks[x][y][z] = nullptr;
		update_neighbors(x, y, z, true);
	}
}