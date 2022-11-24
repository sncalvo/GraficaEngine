#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>    // std::min
#include <cmath>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Particle.h"
#include "Texture.h"
#include "Shader.h"
#include "../Core/PerspectiveCamera.h"

namespace Engine {
    class ParticleSystem {
    public:
        ParticleSystem(glm::vec3 initial_position);
        void update();
        void draw(Camera *camera);
    private:
        std::vector<Particle> particles;
        std::vector<float> vertices;
        unsigned int VAO;
        unsigned int VBO;
        int particle_amount = 10000;
        Shader *shader;
        Texture *texture;
        glm::vec3 _position;
    };
}