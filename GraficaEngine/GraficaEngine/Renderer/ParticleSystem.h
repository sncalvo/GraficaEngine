#pragma once

#include <iostream>
#include <GL/glew.h>

#include "Particle.h"

float parabola(float t) {
    return - 0.2f * t * t;
}

float rand11() {
    return (rand() / (float)RAND_MAX) * 2 - 1;
}

float rand01() {
    return ((double)rand() / RAND_MAX) * 1.01 + 0.01;
}

namespace Engine {
    class ParticleSystem {
    public:
        ParticleSystem();
        void update();
    private:
        std::vector<Particle> particles;
        std::vector<float> vertices;
        unsigned int VAO;
        unsigned int VBO;
        int particle_amount = 1000;
    };
}