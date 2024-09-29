#pragma once

#include <glm/glm.hpp>

#include "Shader.cpp"



/// nieużywane



class PointLight {
public:
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;

    PointLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, 
          float constnt, float lin, float quad) 
          : position(pos), ambient(amb), diffuse(diff), specular(spec), 
            constant(constnt), linear(lin), quadratic(quad) {}

    void sendToShader(Shader* shader, const std::string& prefix) {
        shader->setVec3(prefix + ".position", position);
        shader->setVec3(prefix + ".ambient", ambient);
        shader->setVec3(prefix + ".diffuse", diffuse);
        shader->setVec3(prefix + ".specular", specular);
        shader->setFloat(prefix + ".constant", constant);
        shader->setFloat(prefix + ".linear", linear);
        shader->setFloat(prefix + ".quadratic", quadratic);
    }
};