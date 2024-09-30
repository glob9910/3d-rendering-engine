#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.cpp"

class Model {

public:
    Texture* texture;
    std::vector<Mesh> meshes;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;

    Model() {
        this->position = glm::vec3(0.0f);
        this->scale = glm::vec3(1.0f);
        this->rotation = glm::vec3(0.0f);
    }

    void draw() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->ID);
        for(unsigned int i = 0; i < meshes.size(); i++) {
            meshes[i].draw();
        }
    }
    
    void setTexture(Texture* texture) {
        this->texture = texture;
    }

    void setPosition(glm::vec3 position) {
        this->position = position;
    }

    void setScale(glm::vec3 scale) {
        this->scale = scale;
    }

    void setRotation(glm::vec3 rotation) {
        this->rotation = rotation;
    }

    glm::mat4 getModelMatrix() {
        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::rotate(model, this->rotation);
        model = glm::scale(model, this->scale);

        return glm::translate(model, position);
    }
};

