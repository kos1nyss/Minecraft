#include <GL/glut.h>

#include <vector>
#include <iostream>

#include "block.h"

namespace mc {
	std::vector<int> Block::get_visible_faces(Player& player) {
		std::vector<int> visible_faces;
		Coordinates player_coord = player.get_coordinates();
		float player_coord_array[3] = { player_coord.x, player_coord.y, player_coord.z };
		float coord_array[3] = { coordinates.x, coordinates.y, coordinates.z };
		for (int i = 0; i < 3; i++) {
			int face_n = i * 2;
			float c = player_coord_array[i];
			if (i == 1) c += 2;
			if (c > coord_array[i]) {
				face_n++;
			}

			if (!neighbors[face_n]) {
				visible_faces.push_back(face_n);
			}
		}
		return visible_faces;
	}

	void Block::draw(Player& player) {
		was_drawn = true;
		std::vector<int> faces = get_visible_faces(player);
		for (unsigned int i = 0; i < faces.size(); i++) {
			int current_vertexes[4];
			for (int j = 0; j < 4; j++) {
				current_vertexes[j] = facets[faces[i]][j];
			}

			glPushMatrix();
			glColor3f(color[0], color[1], color[2]);
			glTranslatef(coordinates.x, coordinates.y, coordinates.z);
			glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, &facets[faces[i]]);
			glPopMatrix();
		}
	}

	void Block::set_neighbors(bool* new_neighbors) {
		neighbors = new_neighbors;
	}

	void Block::set_color(int r, int g, int b) {
		color[0] = r / 255.f;
		color[1] = g / 255.f;
		color[2] = b / 255.f;
	}

	void Block::shake_color() {
		for (int i = 0; i < 3; i++) {
			color[i] *= 255;

			color[i] = color[i] - 5 + rand() % 15;
			if (color[0] < 0) {
				color[i] = 0;
			}
			else if (color[2] > 255) {
				color[i] = 255;
			}

			color[i] /= 255.f;
		}
	}

	bool Block::get_was_drawn() {
		return was_drawn;
	}

	void Block::allow_drawing() {
		was_drawn = false;
	}
}