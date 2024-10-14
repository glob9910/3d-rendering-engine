#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.cpp"
#include "Material.cpp"

class Model {

public:
    Material* material;
    std::vector<Mesh> meshes;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    float rotationAngle;

    Model() {
        this->position = glm::vec3(0.0f);
        this->scale = glm::vec3(1.0f);
        this->rotation = glm::vec3(1.0f);
        this->rotationAngle = 0.0f;
    }

    Material* getMaterial() {
        return material;
    }

    std::vector<Mesh> getMeshes() {
        return meshes;
    }
    
    void setMaterial(Material* material) {
        this->material = material;
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

    void setRotationAngle(float angle) {
        this->rotationAngle = angle;
    }

    glm::mat4 getModelMatrix() {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, rotationAngle, rotation);
        model = glm::scale(model, scale);
        return model;
    }
};

