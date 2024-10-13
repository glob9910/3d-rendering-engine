#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "model/Window.cpp"

int WIDTH = 800;
int HEIGHT = 600;

int main() {
    try {
        glfwInit();
        new Window(glfwCreateWindow(WIDTH, HEIGHT, "okno", NULL, NULL));
    }
    catch(Exception* e) {
        return -1;
    }

    return 0;
}
