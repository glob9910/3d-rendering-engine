#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "../Exception.cpp"
#include "levels/Level.cpp"


class Window {

public:
    const int SCR_WIDTH = 800;
    const int SCR_HEIGHT = 600;


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

        glfwFocusWindow(window);
        glEnable(GL_DEPTH_TEST);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
        // tell stb_image.h to flip loaded texture's on the y-axis (before loading model)
        stbi_set_flip_vertically_on_load(true);

        Level* level = new Level(SCR_WIDTH, SCR_HEIGHT, window);

        while(!glfwWindowShouldClose(window)) {
            level->processInput();
            level->render();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
    }
};