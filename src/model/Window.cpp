#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstring>
#include <vector>
#include <utility>

#include "../Exception.cpp"
#include "Shader.cpp"
#include "Camera.cpp"
#include "Model.cpp"
#include "Texture.cpp"
#include "LoadedModel.cpp"
#include "Renderer.cpp"
#include "Box.cpp"
#include "Light.cpp"
#include "PointLight.cpp"

#include "Level.cpp"


class Window {

public:
    const int SCR_WIDTH = 800;
    const int SCR_HEIGHT = 600;

    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;            

    bool firstMouse = true;

    Camera* camera;

    glm::vec3 pointLightPositions[4] {
        glm::vec3( 2.3f, -1.0f, -3.5f),  
        glm::vec3( 1.5f,  -1.0f, 2.5f), 
        glm::vec3( 0.5f,  0.2f, -0.5f), 
        glm::vec3(-4.3f,  4.0f, -1.5f)
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

        glfwFocusWindow(window);
        glEnable(GL_DEPTH_TEST);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetWindowUserPointer(window, this);
        glfwSetCursorPosCallback(window, static_mouse_callback);
        glfwSetCursorPos(window, lastX, lastY);

        // tell stb_image.h to flip loaded texture's on the y-axis (before loading model)
        stbi_set_flip_vertically_on_load(true);

        // shader programs:
        // Shader* ourShader = new Shader("src/model/shaders/shader.vs", "src/model/shaders/shader.fs");
        // Shader* lightningShader = new Shader("src/model/shaders/lightShader.vs", "src/model/shaders/lightShader.fs");
        // Shader* modelShader = new Shader("src/model/shaders/modelShader.vs", "src/model/shaders/modelShader.fs");

        // camera
        camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

        // Renderer* renderer = new Renderer(SCR_WIDTH, SCR_HEIGHT);


        // // lights
        // std::vector<Light*>* lights = new std::vector<Light*>();

        // DirLight* dirLight = new DirLight();
        // dirLight->setAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
        // dirLight->setDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));
        // dirLight->setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
        // dirLight->setDirection(glm::vec3(0.0f, -1.0f, 0.0f));
        // //lights->push_back(dirLight);
    
        // std::vector<PointLight*>* pointLights = new std::vector<PointLight*>();
        // pointLights->push_back(createPointLight(glm::vec3(0.0f, 0.0f, 0.0f)));
        // pointLights->push_back(createPointLight(glm::vec3(1.0f, 1.0f, 1.0f)));

        // std::vector<Model*>* modelsForLightShader = new std::vector<Model*>();
        // for(PointLight* pointLight : *pointLights) {
        //     modelsForLightShader->push_back(pointLight->getModel());
        //     lights->push_back(createPointLight(glm::vec3(0.0f, 0.0f, 0.0f)));
        // }
        // std::pair<Shader*, std::vector<Model*>*>* lightShaderModels = new std::pair<Shader*, std::vector<Model*>*>(lightningShader, modelsForLightShader);

        //models
        // Model* backpack = new LoadedModel("src/model/assets/backpack/backpack.obj", new Texture("src/model/assets/backpack/diffuse.jpg", false));
        // Model* bird = new LoadedModel("src/model/assets/bird/bird.obj", new Texture("src/model/assets/bird/diffuse.jpg", false));
        // Model* knight = new LoadedModel("src/model/assets/knight/knight2.obj", new Texture("src/model/assets/knight/armor.jpg", false));
        // Model* penguin = new LoadedModel("src/model/assets/penguin/penguin.obj", new Texture("src/model/assets/penguin/PenguinDiffuseColor.png", true));

        // backpack->setPosition(glm::vec3(6, 0, 0));
        // bird->setPosition(glm::vec3(15, 0, 0));
        // knight->setPosition(glm::vec3(-8, 0 ,0));
        // penguin->setPosition(glm::vec3(-1, 0, 0));

        // std::vector<Model*>* modelsForModelShader = new std::vector<Model*>();
        // modelsForModelShader->push_back(backpack);
        // modelsForModelShader->push_back(bird);
        // modelsForModelShader->push_back(knight);
        // modelsForModelShader->push_back(penguin);
        // // for(Model* model: *modelsForModelShader) {
        // //     model->getMaterial()->setShininess(1.0f);
        // // }
        // std::pair<Shader*, std::vector<Model*>*>* modelShaderModels = new std::pair<Shader*, std::vector<Model*>*>(ourShader, modelsForModelShader);

        // boxes
        // std::vector<Model*>* modelsForOurShader = new std::vector<Model*>();
        // modelsForOurShader->push_back(createBox(glm::vec3( 2.0f,  5.0f, -15.0f)));
        // modelsForOurShader->push_back(createBox(glm::vec3(-1.5f, -2.2f, -2.5f)));
        // modelsForOurShader->push_back(createBox(glm::vec3(-3.8f, -2.0f, -12.3f)));
        // modelsForOurShader->push_back(createBox(glm::vec3( 2.4f, -0.4f, -3.5f)));
        // modelsForOurShader->push_back(createBox(glm::vec3(-1.7f,  3.0f, -7.5f)));
        // modelsForOurShader->push_back(createBox(glm::vec3( 1.3f, -2.0f, -2.5f)));
        // modelsForOurShader->push_back(createBox(glm::vec3( 1.5f,  2.0f, -2.5f)));
        // modelsForOurShader->push_back(createBox(glm::vec3( 1.5f,  0.2f, -1.5f)));
        // std::pair<Shader*, std::vector<Model*>*>* ourShaderModels = new std::pair<Shader*, std::vector<Model*>*>(ourShader, modelsForOurShader);

        // std::vector<std::pair<Shader*, std::vector<Model*>*>*>* toRender = new std::vector<std::pair<Shader*, std::vector<Model*>*>*>();
        // toRender->push_back(lightShaderModels);
        // toRender->push_back(ourShaderModels);
        // toRender->push_back(modelShaderModels);

        Level* level = new Level(SCR_WIDTH, SCR_HEIGHT);

        while(!glfwWindowShouldClose(window)) {
            processInput(window);
            level->render(camera);
            //renderer->render(toRender, lights, camera);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
    }

    // Box* createBox(glm::vec3 position) {
    //     Box* box = new Box();
    //     box->setPosition(position);
    //     box->setMaterial(new Material(
    //         new Texture("src/model/assets/container2.png", true),
    //         new Texture("src/model/assets/container2_specular.png", true),
    //         32.0f
    //     ));
    //     return box;
    // }

    // PointLight* createPointLight(glm::vec3 position) {
    //     PointLight* pointLight = new PointLight();
    //     pointLight->setPosition(position);
    //     pointLight->setModel(createBox(pointLight->getPosition()));
    //     pointLight->scaleModel(glm::vec3(0.5f, 0.5f, 0.5f));
    //     pointLight->setAmbient(glm::vec3(0.1f, 0.1f, 0.1f));
    //     pointLight->setDiffuse(glm::vec3(0.8f, 0.8f, 0.8f));
    //     pointLight->setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
    //     pointLight->setConstant(1.0f);
    //     pointLight->setLinear(0.09f);
    //     pointLight->setQuadratic(0.032f);   
    //     return pointLight;    
    // }


    void processInput(GLFWwindow *window) {
        const float cameraSpeed = 0.05f;
        const float fovSpeed = 1.0f;

        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        
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
            camera->processMouseScroll(true);
        if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
            camera->processMouseScroll(false);
    }

    static void static_mouse_callback(GLFWwindow* window, double xpos, double ypos) {
        Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
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
};