#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

class Light {
public:

    static unsigned int VAO;


    static unsigned int setVAO() {
        // // VAO
        // glGenVertexArrays(1, &VAO);
        // glBindVertexArray(VAO);

        // // VBO
        // unsigned int VBO;
        // glGenBuffers(1, &VBO);
        // glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // // Vertex attributes structure in VBO
        // // position attribute
        // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        // glEnableVertexAttribArray(0);
        
        return VAO;
    }
};

unsigned int Light::VAO = 0;