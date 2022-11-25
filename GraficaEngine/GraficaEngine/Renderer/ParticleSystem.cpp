#include "ParticleSystem.h"

float BOX_SIDE = 2.f;

namespace Engine {
    int rand12() {
        return floor((rand() / (float)RAND_MAX) * 2.99f) - 1;
    }

    float rand11() {
        return (rand() / (float)RAND_MAX) * 2.f * BOX_SIDE - BOX_SIDE;
    }

    float rand01() {
        return ((double)rand() / RAND_MAX);
    }

    ParticleSystem::ParticleSystem(glm::vec3 intial_position) : _position(intial_position) {
        _speed = 0.1f;
        particle_amount = 1000;
        vertices.reserve(3 * particle_amount);
        particles.reserve(particle_amount);

        for(int i = 0; i < particle_amount; i++) {
            int position_id = i * 3;
            particles.push_back(Particle(position_id, _defaultVelocity));
            vertices.push_back(rand11());
            vertices.push_back(rand11());
            vertices.push_back(rand11());
            types.push_back(rand12());
        }


        glGenVertexArrays(1, &VAO); 
        glBindVertexArray(VAO);

        glGenBuffers(2, VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);  
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);  

        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(int) * types.size(), &types[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 1, GL_INT, GL_FALSE, sizeof(int), (void*) 0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);  
        glBindVertexArray(0);

        shader = new Shader("Assets/Shaders/particles.vs", "Assets/Shaders/particles.gs", "Assets/Shaders/particles.fs");
        texture = new Texture("Assets/Models/shape-white.png", "lol");
    }

   int antiSign(float f) {
     return (f > 0) ? -1 : ((f < 0) ? 1 : 0);
   }

   void newGlobalVelocity(float *velocity, float current_x) {
      float rand = rand11();
      if(rand > current_x) {
        *velocity = 5.f;
      } else if (rand < current_x) {
        *velocity = -5.f;
      } else {
        *velocity = 0.f;
      }
   }

   void newVelocity(float *velocity) {
      float rand = rand01();
      if(rand > 0.6f) {
        *velocity = 0.1f;
      } else if (rand > 0.4f) {
        *velocity = -0.1f;
      } else {
        *velocity = 0.f;
      }
   }

    void ParticleSystem::update(float deltaTime) {
        _timeAcc += deltaTime;
        _timeAcc2 += deltaTime;

        if (_timeAcc2 > 0.5f) {
            _timeAcc2 = 0.f;
            for(int i = 0; i < 10; i++) {
                newVelocity(&speedMods[i]);
            }
        }

        if (_timeAcc > 1.0f) {
            _timeAcc = 0.f;
            newGlobalVelocity(&speedMods[10], particles[0].velocity.x);
        }

        for(int i = 0; i < particle_amount; i++) {
            float globalSpeedMod = speedMods[10];
            float speedMod = speedMods[i % 10];

            int vertices_position = particles[i].position_id;
            particles[i].velocity.x += globalSpeedMod * deltaTime * _speed;
/*             newVelocity(&particle.velocity); */
            glm::vec3 velocity = particles[i].velocity * deltaTime * _speed + speedMod * deltaTime * _speed;
            // Particles x position
            float x = vertices[vertices_position] + velocity.x;
            // Particles y position
            float y = vertices[vertices_position + 1] + velocity.y;
            // Particles z position
            float z = vertices[vertices_position + 2] + velocity.z;

            // Cycle particles
            if (BOX_SIDE < x || x < -BOX_SIDE) {
                vertices[vertices_position] = antiSign(x) * BOX_SIDE;
            } else {
                vertices[vertices_position] = x;
            }

            if (BOX_SIDE < y || y < -BOX_SIDE) {
                vertices[vertices_position + 1] = antiSign(y) * BOX_SIDE;
            } else {
                vertices[vertices_position + 1] = y;
            }

            if (BOX_SIDE < z || z < -BOX_SIDE) {
                vertices[vertices_position + 2] = antiSign(z) * BOX_SIDE;
            } else {
                vertices[vertices_position + 2] = z;
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
        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);

        shader->use();
        texture->activateTextureAs(0);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);  
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);  
        glBufferData(GL_ARRAY_BUFFER, sizeof(int) * types.size(), &types[0], GL_STATIC_DRAW);

		glm::mat4 projection = camera->getProjectionMatrixFor(0.f, 0.5f);
		glm::mat4 view = camera->getViewMatrix();
        glm::vec3 camera_position = camera->transform.position;
        float full_side = BOX_SIDE * 2.f;

        if (glm::distance(camera_position, _position) > full_side) {
          _position = camera_position;
        }

		glm::mat4 model = glm::mat4(1.0f);
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
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
    }
}