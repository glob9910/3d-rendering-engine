#pragma once

#include <vector>

#include "../Window.cpp"
#include "../models/Model.cpp"
#include "../lights/Light.cpp"
#include "../lights/DirLight.cpp"
#include "../lights/PointLight.cpp"
#include "../models/Box.cpp"
#include "../Shader.cpp"
#include "../models/LoadedModel.cpp"
#include "../Camera.cpp"
#include "../models/Skybox.cpp"

class AbstractLevel {
protected:

    std::vector<Light*>* lights;
    std::vector<std::pair<Shader*, std::vector<Model*>*>*>* toRender;
    std::vector<Model*>* collisableModels;

    Skybox* skybox = nullptr;

    GLFWwindow *window;

    Camera* camera;

    float lastX;
    float lastY;           

    bool firstMouse = true;
    bool detectCollisions = true;

public:
    AbstractLevel(int SCR_WIDTH, int SCR_HEIGHT, GLFWwindow *window) {
        this->window = window;
        lastX = SCR_WIDTH / 2.0f;
        lastY = SCR_HEIGHT / 2.0f; 

        glfwSetCursorPosCallback(window, static_mouse_callback);
        glfwSetCursorPos(window, lastX, lastY);

        camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

        lights = new std::vector<Light*>;    
        toRender = new std::vector<std::pair<Shader*, std::vector<Model*>*>*>;
        collisableModels = new std::vector<Model*>;
    }

    void processInput() {
        const float cameraSpeed = 0.05f;
        const float fovSpeed = 1.0f;

        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if(glfwGetKey(window, GLFW_KEY_C))
            detectCollisions = !detectCollisions;

        glm::vec3 lastPosition = camera->position;
        
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera->processKeyboard(FORWARD);
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera->processKeyboard(BACKWARD);
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera->processKeyboard(LEFT);
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera->processKeyboard(RIGHT);
        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera->processKeyboard(UP);
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera->processKeyboard(DOWN);
        if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
            camera->processZoom(true);
        if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
            camera->processZoom(false);

        if(detectCollisions) {
            for(Model* model : *collisableModels) {
                camera->isCollision = detectCameraCollision(model);
                if(camera->isCollision) {
                    camera->position = lastPosition;
                    break;
                }
            }
        }
    }

    virtual void update() {}

    std::vector<Light*>* getLights() {
        return lights;
    }

    std::vector<std::pair<Shader*, std::vector<Model*>*>*>* getModels() {
        return toRender;
    }
    
    Skybox* getSkybox() {
        return skybox;
    }

    Camera* getCamera() {
        return camera;
    }

protected:
    static void static_mouse_callback(GLFWwindow* window, double xpos, double ypos) {
        AbstractLevel* instance = static_cast<AbstractLevel*>(glfwGetWindowUserPointer(window));
        instance->mouse_callback(window, xpos, ypos);
    }

    void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
        if (firstMouse) {
            glfwSetCursorPos(window, lastX, lastY);
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
            
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        camera->processMouseMovement(xoffset, yoffset);
    }

    bool detectCameraCollision(Model* model) {
        glm::vec3 modelMin = model->position - model->scale;
        glm::vec3 modelMax = model->position + model->scale;

        glm::vec3 cameraPosition = camera->position;

        bool collisionX = cameraPosition.x >= modelMin.x && cameraPosition.x <= modelMax.x;
        bool collisionY = cameraPosition.y >= modelMin.y && cameraPosition.y <= modelMax.y;
        bool collisionZ = cameraPosition.z >= modelMin.z && cameraPosition.z <= modelMax.z;

        return collisionX && collisionY && collisionZ;
    }
};