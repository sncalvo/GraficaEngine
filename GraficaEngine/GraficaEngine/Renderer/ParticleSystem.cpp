#include "ParticleSystem.h"


namespace Engine {
    float parabola(float t) {
        return - 0.2f * t * t;
    }

    float rand11() {
        return (rand() / (float)RAND_MAX) * 2 - 1;
    }

    float rand01() {
        return ((double)rand() / RAND_MAX) * 1.02 + 0.01;
    }

    ParticleSystem::ParticleSystem() {
        vertices.reserve(3 * particle_amount);
        particles.reserve(particle_amount);

        for(int i = 0; i < particle_amount; i++) {
            int position_id = i * 3;
            particles.push_back(Particle(position_id));
            vertices.push_back(rand11());
            vertices.push_back(rand11());
            vertices.push_back(rand01() * -1);
        }


        VAO;
        glGenVertexArrays(1, &VAO); 
        glBindVertexArray(VAO);

        VBO;
        glGenBuffers(1, &VBO);  
        glBindBuffer(GL_ARRAY_BUFFER, VBO);  
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);  
        glBindBuffer(GL_ARRAY_BUFFER, 0);  
        shader = new Shader("Assets/Shaders/particles.vs", "Assets/Shaders/particles.gs", "Assets/Shaders/particles.fs");
        texture = new Texture("Assets/Models/shape-white.png", "lol");
        projection = glm::perspective(glm::radians(150.f), 1.f, 0.0f, 1.f); // TODO: Change parameters
        glBindVertexArray(0);
    }


    void ParticleSystem::update() {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable( GL_BLEND );
        for(auto particle : particles) {
            int vertices_position = particle.position_id;
            // Particles x position
            vertices[vertices_position] += particle.velocity.x;
            // Particles z position
            vertices[vertices_position + 2] += particle.velocity.z;

            // Particles y position
            float y = vertices[vertices_position + 1] + particle.velocity.y;
            // If a particle falls bellow what player sees, thorw it to the top
            if (y <= -1.f) {
                vertices[vertices_position + 1] = 1.f;
            } else {
                vertices[vertices_position + 1] = y;
            }

        }


        glBindBuffer(GL_ARRAY_BUFFER, VBO);  
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

        shader->use();

        shader->setMat4("projection", projection);
        texture->activateTextureAs(0);
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, particle_amount);
        glDisable( GL_BLEND );
    }
}