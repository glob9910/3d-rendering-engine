#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Texture.cpp"
#include "Shader.cpp"
#include <vector>

#include "Cube.cpp"

    glm::vec3 boxPositions[10] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };


class Box {
public:
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;

    static Texture* diffuseTexture;
    static Texture* specularTexture;
    static unsigned int VAO;
    static Shader* boxShader;


    Box(glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f), glm::vec3 rotation = glm::vec3(0.0f)) {
        this->position = position;
        this->scale = scale;
        this->rotation = rotation;
    }

    static void setTexture(Texture* diffuse, Texture* specular) {
        diffuseTexture = diffuse;
        specularTexture = specular;
    }

    static void setShader(Shader* shader) {
        boxShader = shader;
    }

    void draw(Shader* shader = Box::boxShader) {
        shader->use();

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, diffuseTexture->ID);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, specularTexture->ID);

        glBindVertexArray(VAO);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);

        //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

        shader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }


    static unsigned int setVAO() {
        // VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // VBO
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // vertex attributes structure in VBO
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // normal attribute
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        
        // texture attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(1);

  
        return VAO;
    }
};

// Initialize static members
Texture* Box::diffuseTexture = nullptr;
Texture* Box::specularTexture = nullptr;
unsigned int Box::VAO = 0;
Shader* Box::boxShader = nullptr;
