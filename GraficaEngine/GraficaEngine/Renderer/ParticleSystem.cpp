#include "ParticleSystem.h"


namespace Engine {
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
        Engine::Shader shader("Assets/Shaders/particles.vs", "Assets/Shaders/particles.gs", "Assets/Shaders/particles.fs");
        Engine::Texture texture("Assets/Models/shape-white.png", "lol");
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable( GL_BLEND );
        projection = glm::perspective(glm::radians(150.f), 1.f, 0.0f, 1.f); // TODO: Change parameters
    }


    void ParticleSystem::update() {
        for(auto particle : particles) {
            int vertices_position = particle.position_id;
            vertices[vertices_position] += particle.velocity.x;
            vertices[vertices_position + 1] += particle.velocity.y;
            vertices[vertices_position + 2] += particle.velocity.z;
        }


        glBindBuffer(GL_ARRAY_BUFFER, VBO);  
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

        std::cout << (float)(rand01() * -1) << std::endl;

        shader.use();

        shader.setMat4("projection", projection);
        texture.activateTextureAs(0);
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, particle_amount);
    }
}