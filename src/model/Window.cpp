#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "../Exception.cpp"
#include "levels/Level.cpp"
#include "levels/Level2.cpp"
#include "Renderer.cpp"

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

        Renderer* renderer = new Renderer(SCR_WIDTH, SCR_HEIGHT);
        std::vector<AbstractLevel*>* levels = new std::vector<AbstractLevel*>;
        levels->push_back(new Level(SCR_WIDTH, SCR_HEIGHT, window));
        levels->push_back(new Level2(SCR_WIDTH, SCR_HEIGHT, window));
        AbstractLevel* level = levels->at(0);

        while(!glfwWindowShouldClose(window)) {
            if(glfwGetKey(window, GLFW_KEY_RIGHT)) {
                level = levels->at(1);
            }
            if(glfwGetKey(window, GLFW_KEY_LEFT)) {
                level = levels->at(0);
            }
            glfwSetWindowUserPointer(window, level);
            level->processInput();
            level->update();
            renderer->render(level);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
    }
};