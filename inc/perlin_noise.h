#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

namespace mc {
    class PerlinNoise {
    private:
        int size;
        float* noise_function(int length, int dots, float amplitude);
        float** noise_matrix(int size, int dots, float amplitude);

    public:
        float** create(int size);
    };
}

#endif