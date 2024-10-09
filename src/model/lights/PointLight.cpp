#pragma once

#include "glm/glm.hpp"

#include "Light.cpp"
#include "../models/Model.cpp"

class PointLight : public Light {
public:
    glm::vec3 position;
    Model* model;

    float constant;
    float linear;
    float quadratic;
    
    void setModel(Model* model) {
        this->model = model;
    }

    void setPosition(glm::vec3 position) {
        this->position = position;
    }

    void setConstant(float constant) {
        this->constant = constant;
    }

    void setLinear(float linear) {
        this->linear = linear;
    }

    void setQuadratic(float quadratic) {
        this->quadratic = quadratic;
    }

    glm::vec3 getPosition() {
        return position;
    }

    float getConstant() {
        return constant;
    }

    float getLinear() {
        return linear;
    }

    float getQuadratic() {
        return quadratic;
    }

    Model* getModel() {
        return this->model;
    }

    LIGHT getType() {
        return LIGHT::POINT;
    }
};