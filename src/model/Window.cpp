#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstring>

#include "../Exception.cpp"
#include "Shader.cpp"
#include "Camera.cpp"

#include "Box.cpp"

#include "Model.cpp"

#include "Texture.cpp"



class Window {

public:
    const int SCR_WIDTH = 800;
    const int SCR_HEIGHT = 600;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int lightVAO;

    
    Shader* ourShader;
    Shader* lightningShader;
    Shader* modelShader;

    Camera* camera;
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;

    Model* backpackModel;
    Model* birdModel;
    Model* penguinModel;
    Model* knightModel;
    Texture* birdTexture;
    Texture* backpackTexture;
    Texture* penguinTexture;
    Texture* knightTexture;

    std::vector<Box> boxes;
    

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

        // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
        stbi_set_flip_vertically_on_load(true);

        // model
        // build and compile our shaderProgram
        ourShader = new Shader("src/model/shaders/shader.vs", "src/model/shaders/shader.fs");
        lightningShader = new Shader("src/model/shaders/lightShader.vs", "src/model/shaders/lightShader.fs");
        modelShader = new Shader("src/model/shaders/modelShader.vs", "src/model/shaders/modelShader.fs");

        //boxes
        createBoxes();

        // lightning VAO
        glGenVertexArrays(1, &lightVAO);
        glBindVertexArray(lightVAO);
        // leaving the VBO the same as for the normal objects
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        
        // camera
        camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

        // textures for models
        birdTexture = new Texture("src/model/assets/bird/diffuse.jpg", false);
        backpackTexture = new Texture("src/model/assets/backpack/diffuse.jpg", false);
        penguinTexture = new Texture("src/model/assets/penguin/PenguinDiffuseColor.png", true);
        knightTexture = new Texture("src/model/assets/knight/armor.jpg", false);

        // downloaded models
        backpackModel = new Model("src/model/assets/backpack/backpack.obj", *backpackTexture);
        birdModel = new Model("src/model/assets/bird/bird.obj", *birdTexture);
        penguinModel = new Model("src/model/assets/penguin/penguin.obj", *penguinTexture);
        knightModel = new Model("src/model/assets/knight/knight2.obj", *knightTexture);
        //setTexture(birdTexture, "src/model/assets/bird/diffuse.jpg", false);
        
        ourShader->use();
        // ourShader->setInt("texture1", 0);
        // ourShader->setInt("texture2", 1);
        ourShader->setInt("material.diffuse", 2);
        ourShader->setInt("material.specular", 3);
        
        // set material
        glm::vec3 ambient = glm::vec3(1.0f, 0.5f, 0.31f);
        glm::vec3 diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
        glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
        float shininess = 32.0f;
        //ourShader->setVec3("material.ambient", ambient);
        ourShader->setVec3("material.diffuse", diffuse);
        ourShader->setVec3("material.specular", specular);
        ourShader->setFloat("material.shininess", shininess);
        
        // set light
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 lightDirection = glm::vec3(0.0f, -1.0f, 0.0f);
        ambient = glm::vec3(0.2f, 0.2f, 0.2f);
        diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
        specular = glm::vec3(1.0f, 1.0f, 1.0f);
        ourShader->setVec3("dirLight.direction", camera->front);
        ourShader->setVec3("dirLight.ambient", ambient);
        ourShader->setVec3("dirLight.diffuse", diffuse);
        ourShader->setVec3("dirLight.specular", specular);
        
        // point light 1
        ourShader->setVec3("pointLight[0].position", pointLightPositions[0]);
        ourShader->setVec3("pointLight[0].ambient", ambient);
        ourShader->setVec3("pointLight[0].diffuse", diffuse);
        ourShader->setVec3("pointLight[0].specular", specular);
        ourShader->setFloat("pointLight[0].constant", 1.0f);
        ourShader->setFloat("pointLight[0].linear", 0.09f);
        ourShader->setFloat("pointLight[0].quadratic", 0.032f);
        // point light 2
        ourShader->setVec3("pointLight[1].position", pointLightPositions[1]);
        ourShader->setVec3("pointLight[1].ambient", ambient);
        ourShader->setVec3("pointLight[1].diffuse", diffuse);
        ourShader->setVec3("pointLight[1].specular", specular);
        ourShader->setFloat("pointLight[1].constant", 1.0f);
        ourShader->setFloat("pointLight[1].linear", 0.09f);
        ourShader->setFloat("pointLight[1].quadratic", 0.032f);
        // point light 3
        ourShader->setVec3("pointLight[2].position", pointLightPositions[2]);
        ourShader->setVec3("pointLight[2].ambient", ambient);
        ourShader->setVec3("pointLight[2].diffuse", diffuse);
        ourShader->setVec3("pointLight[2].specular", specular);
        ourShader->setFloat("pointLight[2].constant", 1.0f);
        ourShader->setFloat("pointLight[2].linear", 0.09f);
        ourShader->setFloat("pointLight[2].quadratic", 0.032f);
        // point light 4
        ourShader->setVec3("pointLight[3].position", pointLightPositions[3]);
        ourShader->setVec3("pointLight[3].ambient", ambient);
        ourShader->setVec3("pointLight[3].diffuse", diffuse);
        ourShader->setVec3("pointLight[3].specular", specular);
        ourShader->setFloat("pointLight[3].constant", 1.0f);
        ourShader->setFloat("pointLight[3].linear", 0.09f);
        ourShader->setFloat("pointLight[3].quadratic", 0.032f);
        // spotLight
        ourShader->setVec3("spotLight.position", camera->position);
        ourShader->setVec3("spotLight.direction", camera->front);
        ambient = glm::vec3(0.0f, 0.0f, 0.0f);
        diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
        specular = glm::vec3(1.0f, 1.0f, 1.0f);
        ourShader->setVec3("spotLight.ambient", ambient);
        ourShader->setVec3("spotLight.diffuse", diffuse);
        ourShader->setVec3("spotLight.specular", specular);
        ourShader->setFloat("spotLight.constant", 1.0f);
        ourShader->setFloat("spotLight.linear", 0.09f);
        ourShader->setFloat("spotLight.quadratic", 0.032f);
        ourShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        ourShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));     

        ourShader->setVec3("viewPos", camera->position);
       

        while(!glfwWindowShouldClose(window)) {
            processInput(window);
            
            render();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
    }

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

    void render() {
        // clear the colorbuffer and zbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        // activate normal shader
        // projection/view
        glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera->GetViewMatrix();
        
        ourShader->use();
        ourShader->setMat4("projection", projection);
        ourShader->setMat4("view", view);

        // transform and draw boxes
        glBindVertexArray(VAO);
        for(int i = 0; i<10; i++) {
            boxes[i].draw();
        }
        
        // //draw downloaded model
        modelShader->use();
        modelShader->setMat4("projection", projection);
        modelShader->setMat4("view", view);
        
        drawModel(backpackModel, backpackTexture, glm::vec3(15.0f, 0.0f, 0.0f), glm::vec3(1.0f));
        drawModel(birdModel, birdTexture, glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.25f));
        drawModel(penguinModel, penguinTexture, glm::vec3(20.0f, 0.0f, 0.0f), glm::vec3(1.0f));
        drawModel(knightModel, knightTexture, glm::vec3(-10.0f, 0.0f, 0.0f), glm::vec3(5.0f));


        // lamp object and activate light shader
        lightningShader->use();
        lightningShader->setMat4("projection", projection);
        lightningShader->setMat4("view", view);
        glBindVertexArray(lightVAO);
        for(unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            
            lightningShader->setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    void drawModel(Model* model, Texture* texture, glm::vec3 position, glm::vec3 scale) {
            // Aktywuj odpowiedni shader dla modelu
            modelShader->use();
            
            // Ustaw teksturę
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture->ID);
            
            // Tworzenie macierzy modelu
            glm::mat4 modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, position);
            modelMatrix = glm::scale(modelMatrix, scale);
            
            // Ustaw model
            modelShader->setMat4("model", modelMatrix);
            
            // Rysowanie modelu
            model->draw();
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

    void createBoxes() {

        // set texture
        Box::setTexture(
            new Texture("src/model/assets/container2.png", true),               // diffuse texture
            new Texture("src/model/assets/container2_specular.png", true)       // specular texture
        );

        // set VAO
        VAO = Box::setVAO();

        // set shader
        Box::setShader(ourShader);
        
        // create boxes
        for(int i=0; i<sizeof(boxPositions) / sizeof(boxPositions[0]); i++) {
            boxes.push_back(Box(boxPositions[i]));
        }
    }
};