#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Particle.h"
#include "Texture.h"
#include "Shader.h"

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
        glm::mat4 projection;
        Shader *shader;
        Texture *texture;
    };
}