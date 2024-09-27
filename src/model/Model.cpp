#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
// #include <assimp/Importer.hpp>
// #include <assimp/scene.h>
// #pragma once

// #include <assimp/postprocess.h>



#include "Shader.cpp"
#include "Mesh.cpp"
#include "ModelLoader.cpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model 
{
public:
    // model data 
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;
    ModelLoader loader;

    // constructor, expects a filepath to a 3D model.
    Model(string const &objPath, Texture &texture) : gammaCorrection(gamma)
    {
        meshes.push_back(loader.load(objPath));
        textures_loaded.push_back(texture);
    }

    // draws the model, and thus all its meshes
    void Draw(Shader &shader)
    {
        for(unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }
    
    Texture* GetTexture(int nr) {
        return &textures_loaded[nr];
    }
};

