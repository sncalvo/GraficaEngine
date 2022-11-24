#pragma once

#include <glm/glm.hpp>

namespace Engine {
    struct Particle {
        int position_id;
        glm::vec3 velocity;
        glm::vec4 color;
        float     life;

        Particle(int position_id) 
        : position_id(position_id), velocity(glm::vec3(-0.005f, -0.010f, 0.f)), color(1.f), life(0.f) { }
    };  
}
