#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h> 
#include <string>
#include <stb_image.h>
#include "Shader.cpp"
#include <glm/glm.hpp>



class Texture {
public:
    unsigned int ID;
    std::string sourceFile;
    bool isPNG;

    Texture(std::string sourceFile, bool isPNG) {
        this->sourceFile = sourceFile;
        this->isPNG = isPNG;

        setTexture();
    }

    void setTexture() {
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);  
        
        // set the texture wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set the texture filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load and generate the texture
        int width, height, nrChannels;
        unsigned char *data = stbi_load(sourceFile.c_str(), &width, &height, &nrChannels, 0);
        if(data) {
            if(isPNG) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }
            else {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed to load texture" << sourceFile << std::endl;
        }
        
        // free image memory
        stbi_image_free(data);
    } 
};