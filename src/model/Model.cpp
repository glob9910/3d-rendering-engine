#pragma once

#include <glm/glm.hpp>

#include "Mesh.cpp"
#include "ModelLoader.cpp"


class Model {

public:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    ModelLoader loader;


    Model(std::string const &objPath, Texture &texture) {
        meshes.push_back(loader.load(objPath));
        textures_loaded.push_back(texture);
    }

    void draw() {
        for(unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].draw();
    }
    
    Texture* getTexture(int nr) {
        return &textures_loaded[nr];
    }
};

