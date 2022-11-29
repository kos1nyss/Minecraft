#include <GL/glut.h>

#define _USE_MATH_DEFINES

#include <math.h>
#include <random>
#include <iostream>
#include<ctime>


#include "block.h"
#include "world.h"
#include "player.h"
#include "ray.h"

using namespace mc;

bool KEY_STATES[257];
int WINDOW_SIZE[2] = { 1024, 1024 };

float FRAME_END_TIME = 0, DELTA_TIME = 0;

Player PLAYER;
World WORLD;

void mouse_motion(int ax, int ay) {
    float dx = WINDOW_SIZE[0] / 2.f - ax, dy = WINDOW_SIZE[1] / 2.f - ay;
    mc::Angle player_angle = PLAYER.get_angle();
    player_angle.y -= dx / 20.f;
    player_angle.x -= dy / 20.f;
    PLAYER.set_angle(player_angle.x, player_angle.y);

    glutWarpPointer(WINDOW_SIZE[0] / 2, WINDOW_SIZE[1] / 2);
}

void mouse_click(int button, int state, int x, int y) {
    if (state != 0) return;
    Ray ray;
    Angle angle = PLAYER.get_angle();
    Coordinates player_coord = PLAYER.get_coordinates();
    if ((button == GLUT_LEFT_BUTTON) || (button == GLUT_RIGHT_BUTTON)) {
        ray.cast(WORLD, 20,
            player_coord.x, player_coord.y + 2, player_coord.z,
            -angle.x, angle.y);
        std::vector<Coordinates> history = ray.get_history();
        unsigned int size = history.size();

        for (int i = size - 1; i >= size - 2; i--) {
            Coordinates c = history[i];
            Block* block = WORLD.get_block(c.x, c.y, c.z);
            if (i == (size - 1)) {
                if (button == GLUT_LEFT_BUTTON && block) {
                    WORLD.delete_block(c.x, c.y, c.z);
                    break;
                }
                else if (!block) {
                    break;
                }
            }
            else if (i == (size - 2) && !block)
            {
                WORLD.new_block(c.x, c.y, c.z);
            }
        }
    }
}

void key_down(unsigned char key, int x, int y) {
    KEY_STATES[key] = true;
}

void key_up(unsigned char key, int x, int y) {
    KEY_STATES[key] = false;
}

void move_player() {
    Ray ray;
    std::vector<Coordinates> history;
    Angle player_angle = PLAYER.get_angle();
    Coordinates player_coord = PLAYER.get_coordinates();

    float speed, angle, dx, dz;
    char chars[9] = "WwSsDdAa";
    for (int i = 0; i < 2; i++)
    {
        speed = 0;

        if (KEY_STATES[chars[4 * i]] || KEY_STATES[chars[4 * i + 1]]) {
            speed = 7;
        }
        if (KEY_STATES[chars[4 * i + 2]] || KEY_STATES[chars[4 * i + 3]]) {
            speed = -7;
        }

        if (i == 0) {
            angle = player_angle.y / 180 * M_PI - M_PI_2;
        }
        else if (i == 1) {
            angle = player_angle.y / 180 * M_PI;
            speed *= 0.7f;
        }

        if (speed != 0) {
            speed *= DELTA_TIME;
            bool move_is_possible = true;
            for (int i = 0.5; i < 2; i++) {
                if (move_is_possible) {
                    for (int a = 0; a < 360; a += 60) {
                        ray.cast(WORLD, 0.05f, player_coord.x + cos(angle) * speed, player_coord.y + i, player_coord.z + sin(angle) * speed, 0, a);
                        history = ray.get_history();
                        unsigned int size = history.size();
                        if (size > 0) {
                            Coordinates c = history[history.size() - 1];
                            if (WORLD.within_the_world(c.x, c.y, c.z) &&
                                WORLD.get_block(c.x, c.y, c.z)) {
                                move_is_possible = false;
                                break;
                            }
                        }
                    }
                }
            }

            if (move_is_possible) {
                PLAYER.move(cos(angle) * speed, 0, sin(angle) * speed);
            }
        }
    }

    player_coord = PLAYER.get_coordinates();
    if (KEY_STATES[' ']) {
        MoveY move_y = PLAYER.get_move_y();
        ray.cast(WORLD, 0.1f, player_coord.x, player_coord.y, player_coord.z, -90, 0);
        std::vector<Coordinates> history = ray.get_history();
        unsigned int size = history.size();
        if (size > 0) {
            Coordinates c = history[history.size() - 1];
            if (WORLD.within_the_world(c.x, c.y, c.z) && WORLD.get_block(c.x, c.y, c.z)) {
                PLAYER.set_y_speed(8);
            }
        }
    }

    MoveY move_y = PLAYER.get_move_y();
    if (move_y.speed > 0) {
        ray.cast(WORLD, 0.1f, player_coord.x, player_coord.y + 2, player_coord.z, 90, 0);
    }
    else {
        ray.cast(WORLD, 0.1f, player_coord.x, player_coord.y, player_coord.z, -90, 0);
    }
    history = ray.get_history();
    unsigned int size = history.size();

    if (size > 0) {
        Coordinates c = history[history.size() - 1];
        if (WORLD.within_the_world(c.x, c.y, c.z) && WORLD.get_block(c.x, c.y, c.z)) {
            PLAYER.set_y_speed(0);

            if (move_y.speed > 0) {
                PLAYER.set_coordinates(player_coord.x, c.y - 3, player_coord.z);
            }
            else {
                PLAYER.set_coordinates(player_coord.x, c.y + 1, player_coord.z);
            }
        }
        else {
            PLAYER.update_y(DELTA_TIME);
            PLAYER.move(0, move_y.speed * DELTA_TIME, 0);
        }
    }

    player_coord = PLAYER.get_coordinates();
    if (player_coord.y < 1) {
        PLAYER.move(1, 48, 1);
    }
}

void transform_matrix() {
    Angle player_angle = PLAYER.get_angle();
    Coordinates player_coord = PLAYER.get_coordinates();
    glRotatef(player_angle.x, 1, 0, 0);
    glRotatef(player_angle.y, 0, 1, 0);
    glTranslatef(-player_coord.x, -player_coord.y - 2, -player_coord.z);
}

void update() {
    glClearColor(0.52f, 0.8f, 0.92f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (DELTA_TIME < 0.5f) {
        glPushMatrix();
        move_player();
        transform_matrix();
        WORLD.draw(PLAYER);
        glPopMatrix();
    }

    glutSwapBuffers();
    std::cout << "FPS: " << 1 / DELTA_TIME << std::endl;

    DELTA_TIME = (glutGet(GLUT_ELAPSED_TIME) - FRAME_END_TIME) / 1000.f;
    FRAME_END_TIME = glutGet(GLUT_ELAPSED_TIME);
}


int main(int argc, char** argv) {
    PLAYER.set_coordinates(100, 40, 100);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(80, 80);
    glutInitWindowSize(WINDOW_SIZE[0], WINDOW_SIZE[1]);
    glutCreateWindow("Minecraft SuperAlpha 0.1v");
    glEnable(GL_DEPTH_TEST);
    glFrustum(-0.25, 0.25, -0.25, 0.25, 0.5f, 250);

    glClearColor(0.52f, 0.8f, 0.92f, 0.0f);

    srand(time(NULL));
    WORLD.generate();

    glutMotionFunc(mouse_motion);
    glutPassiveMotionFunc(mouse_motion);
    glutMouseFunc(mouse_click);
    glutKeyboardFunc(key_down);
    glutKeyboardUpFunc(key_up);
    glutDisplayFunc(update);
    glutIdleFunc(update);

    glutMainLoop();
}