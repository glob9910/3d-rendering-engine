#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../Exception.cpp"
#include "Model.cpp"

class Window {

public:
Model* model;

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

    model = new Model();
    model->bind();

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
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //glDrawArrays(GL_TRIANGLES, 0, 4);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
}

};