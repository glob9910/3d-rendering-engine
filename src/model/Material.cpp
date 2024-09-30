#pragma once

#include "Texture.cpp"

class Material
{
public:
    Texture* diffuse;
    Texture* specular;
    float shininess;

    Material(Texture* diffuse, Texture* specular, float shininess) {
        this->diffuse = diffuse;
        this->specular = specular;
        this->shininess = shininess;
    }

    bool hasSpecular() {
        return this->specular != nullptr;
    }

    Texture* getDiffuse() {
        return diffuse;
    }

    Texture* getSpecular() {
        return specular;
    }

    float getShinines() {
        return shininess;
    }
};
