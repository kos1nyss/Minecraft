#include "perlin_noise.h"

#include <iostream>
#include <random>

#define _USE_MATH_DEFINES
#include <math.h>

namespace mc {
    float* PerlinNoise::noise_function(int length, int dots, float amplitude) {
        int dots_delta = length / dots;
        amplitude *= 1000;

        float* noise = new float[length];
        float current_noise;
        for (int n = 0; n < length + 1; n = n + dots_delta) {
            current_noise = (std::rand() % (int)amplitude) / 1000.0f;
            if (n < length) {
                noise[n] = current_noise;
            }

            if (n > 0) {
                for (int i = 0; i < dots_delta; i++) {
                    noise[i + n - dots_delta] = noise[n - dots_delta] + (current_noise - noise[n - dots_delta]) * (sin((float)i / (float)dots_delta * M_PI - M_PI_2) + 1) / 2;
                }
            }
        }
        return noise;
    }

    float** PerlinNoise::noise_matrix(int size, int dots, float amplitude) {
        float** noise = new float* [size];
        for (int i = 0; i < size; i++) {
            noise[i] = new float[size];
        }

        float* noise_1 = noise_function(size, dots, amplitude);
        float* noise_2 = noise_function(size, dots, amplitude);

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                noise[i][j] = noise_1[i] * noise_2[j];
            }
        }
        delete[] noise_1;
        delete[] noise_2;
        return noise;
    }

    float** PerlinNoise::create(int size) {
        int dots[4] = { 30, 10, 100, 100 };
        float amplitudes[4] = { 1, 1 / 2.f,  1 / 4.f,  1 / 8.f };

        float** noise = noise_matrix(size, dots[0], amplitudes[0]);
        for (int n = 1; n < 4; n++) {
            float** temp_noise = noise_matrix(size, dots[n], amplitudes[n]);
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    noise[i][j] += temp_noise[i][j];
                }
            }
            delete[] temp_noise;
        }
        return noise;
    }
}