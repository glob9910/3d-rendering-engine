#pragma once

#include "Light.cpp"

class DirLight : public Light {
public:
    glm::vec3 direction;

    void setDirection(glm::vec3 direction) {
        this->direction = direction;
    }

    glm::vec3 getDirection() {
        return direction;
    }

    LIGHT getType() {
        return LIGHT::DIR;
    }
};