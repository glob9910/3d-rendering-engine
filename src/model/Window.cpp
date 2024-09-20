#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include "../Exception.cpp"
#include "Shader.cpp"


class Window {

public:
    unsigned int VAO;
    Shader* ourShader;

    float vertices[18] = {
        // positions         // colors
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };   
    unsigned int indices[9] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3,    // second triangle
        0, 4, 2
    };  

    

Window(GLFWwindow* window) {
    if(window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw new Exception();
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        throw new Exception();
    }    

    // model
    // build and compile our shaderProgramm
    ourShader = new Shader("shaders/shader.vs", "shaders/shader.fs");

    // vertex array object (VAO) - italian siren
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // vertex buffer objects (VBO)
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // vertex atributes structure in VBO
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // element buffer object (EBO) - for indexed vertices - less memory consumption for bigger models
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // unbind VAO etc.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void render() {
    // clear the colorbuffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // be sure to activate the shader
    ourShader->use();
  
    // now render the triangle
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

};