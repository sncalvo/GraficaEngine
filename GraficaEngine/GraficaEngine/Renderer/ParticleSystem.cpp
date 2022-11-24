#include "ParticleSystem.h"

float BOX_SIDE = 5.f;

namespace Engine {
    float rand11() {
        return (rand() / (float)RAND_MAX) * 2 * BOX_SIDE - BOX_SIDE;
    }

    float rand01() {
        return ((double)rand() / RAND_MAX);
    }

    ParticleSystem::ParticleSystem(glm::vec3 intial_position) : _position(intial_position) {
        vertices.reserve(3 * particle_amount);
        particles.reserve(particle_amount);

        for(int i = 0; i < particle_amount; i++) {
            int position_id = i * 3;
            particles.push_back(Particle(position_id));
            vertices.push_back(rand11());
            vertices.push_back(rand11());
            vertices.push_back(rand11());
        }


        glGenVertexArrays(1, &VAO); 
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);  
        glBindBuffer(GL_ARRAY_BUFFER, VBO);  
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(0);  
        glBindBuffer(GL_ARRAY_BUFFER, 0);  
        glBindVertexArray(0);

        shader = new Shader("Assets/Shaders/particles.vs", "Assets/Shaders/particles.gs", "Assets/Shaders/particles.fs");
        texture = new Texture("Assets/Models/shape-white.png", "lol");
    }


    void ParticleSystem::update() {
        for(auto particle : particles) {
            int vertices_position = particle.position_id;
            // Particles x position
            vertices[vertices_position] += particle.velocity.x;
            // Particles z position
            vertices[vertices_position + 2] += particle.velocity.z;

            // Particles y position
            float y = vertices[vertices_position + 1] + particle.velocity.y;
            // If a particle falls bellow what player sees, thorw it to the top
            if (y <= -BOX_SIDE) {
                vertices[vertices_position + 1] = BOX_SIDE;
            } else {
                vertices[vertices_position + 1] = y;
            }
        }
    }

    void drawParticleBox(glm::mat4 model, Shader *shader, int particle_amount, float coord_position_diff) {
        shader->setMat4("model", model);
        int factor = std::floor((coord_position_diff / BOX_SIDE) * (float)particle_amount);
        int final_amount = std::min(factor, particle_amount);
        glDrawArrays(GL_POINTS, 0, final_amount);
    }

    void ParticleSystem::draw(Camera *camera) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);  
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

        shader->use();

		glm::mat4 projection = camera->getProjectionMatrixFor(0.f, 0.5f);
		glm::mat4 view = camera->getViewMatrix();
        float full_side = BOX_SIDE * 2.f;

		glm::mat4 model = glm::mat4(1.0f);
        glm::vec3 camera_position = camera->transform.position;
        glm::vec3 position_diff = camera_position - _position;
        if (position_diff.x > BOX_SIDE) {
          _position.x += full_side;
        } else if (position_diff.x < -BOX_SIDE) {
          _position.x -= full_side;
        }

        if (position_diff.y > BOX_SIDE) {
          _position.y += full_side;
        } else if (position_diff.y < -BOX_SIDE) {
          _position.y -= full_side;
        }

        if (position_diff.z > BOX_SIDE) {
          _position.z += full_side;
        } else if (position_diff.z < -BOX_SIDE) {
          _position.z -= full_side;
        }

        shader->setMat4("projection", projection);
        shader->setMat4("view", view);
        model = glm::translate(model, _position);

        drawParticleBox(model, shader, particle_amount, BOX_SIDE);

        // Draw closer snow boxes
        bool closer_x_pos = position_diff.x > 0.f;
        bool closer_y_pos = position_diff.y > 0.f;
        bool closer_z_pos = position_diff.z > 0.f;
        glm::vec3 offset;

        glm::mat4 transformed_x, transformed_y, transformed_z, transformed_xy;
        float coord_position_diff;
        
        // Offset in x
        if (closer_x_pos) {
            offset = glm::vec3(full_side, 0.f, 0.f);
        } else {
            offset = glm::vec3(-full_side, 0.f, 0.f);
        }

        coord_position_diff = glm::abs(position_diff.x);
        transformed_x = glm::translate(model, offset);
        drawParticleBox(transformed_x, shader, particle_amount, coord_position_diff);

        // Offset in y
        if (closer_y_pos) {
            offset = glm::vec3(0.f, full_side, 0.f);
        } else {
            offset = glm::vec3(0.f, -full_side, 0.f);
        }

        coord_position_diff = glm::abs(position_diff.y);
        transformed_y = glm::translate(model, offset);
        drawParticleBox(transformed_y, shader, particle_amount, coord_position_diff);

        transformed_xy = glm::translate(transformed_x, offset);
        drawParticleBox(transformed_xy, shader, particle_amount, coord_position_diff);

        // Offset in z
        if (closer_z_pos) {
            offset = glm::vec3(0.f, 0.f, full_side);
        } else {
            offset = glm::vec3(0.f, 0.f, -full_side);
        }
        coord_position_diff = glm::abs(position_diff.z);
        transformed_z = glm::translate(model, offset);
        drawParticleBox(transformed_z, shader, particle_amount, coord_position_diff);

        transformed_z = glm::translate(transformed_x, offset);
        drawParticleBox(transformed_z, shader, particle_amount, coord_position_diff);

        transformed_z = glm::translate(transformed_y, offset);
        drawParticleBox(transformed_z, shader, particle_amount, coord_position_diff);

        transformed_z = glm::translate(transformed_xy, offset);
        drawParticleBox(transformed_z, shader, particle_amount, coord_position_diff);

        glBindBuffer(GL_ARRAY_BUFFER, 0);  
        glBindVertexArray(0);
/*         glDisable( GL_BLEND ); */
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }
}