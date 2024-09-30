#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

enum LIGHT {
    BASIC,
    DIR,
    POINT
};

class Light {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    void setAmbient(glm::vec3 ambient) {
        this->ambient = ambient;
    }

    void setDiffuse(glm::vec3 diffuse) {
        this->diffuse = diffuse;
    }

    void setSpecular(glm::vec3 specular) {
        this->specular = specular;
    }

    glm::vec3 getAmbient() {
        return ambient;
    }

    glm::vec3 getDiffuse() {
        return diffuse;
    }

    glm::vec3 getSpecular() {
        return specular;
    }

    virtual LIGHT getType() {
        return LIGHT::BASIC;
    }
};