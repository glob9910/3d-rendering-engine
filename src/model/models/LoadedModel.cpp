#pragma once

#include <glm/glm.hpp>

#include "Model.cpp"
#include "Texture.cpp"
#include "ModelLoader.cpp"


class LoadedModel : public Model {
public:
    ModelLoader loader;

    LoadedModel(std::string objPath, Texture* texture) {
        this->position = glm::vec3(0.0f);
        this->scale = glm::vec3(1.0f);
        this->rotation = glm::vec3(0.0f);
        meshes.push_back(loader.load(objPath));
        this->setMaterial(new Material(texture, nullptr, 16));
    }
};

