#pragma once

#include <glm/glm.hpp>

#include "Model.cpp"
#include "Texture.cpp"
#include "ModelLoader.cpp"


class LoadedModel : public Model {
public:
    ModelLoader loader;

    LoadedModel(std::string objPath, Texture* texture) {
        Model();
        meshes.push_back(loader.load(objPath));
        this->setMaterial(new Material(texture, nullptr, 16));
    }
};

