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
        void update(float detltaTime);
        void draw(Camera *camera);
        void setSpeed(float speed) { _speed = speed; }
    private:
        float _timeAcc = 0.f;
        float _timeAcc2 = 0.f;
        int _acc = 0;
        float speedMods[11] = {0.f};
        std::vector<Particle> particles;
        std::vector<float> vertices;
        std::vector<int> types;
        unsigned int VAO;
        unsigned int VBO[2];
        int particle_amount = 1000;
        Shader *shader;
        Texture *texture;
        glm::vec3 _position;
        glm::vec3 _defaultVelocity = glm::normalize(glm::vec3(-0.1f, -0.9f, 0.f));
        float _speed = 0.1f;
    };
}