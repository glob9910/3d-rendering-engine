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

#include "Cube.cpp"
#include "Light.cpp"

#include "Model.cpp"

#include "Texture.cpp"


class Window {

public:
    const int SCR_WIDTH = 800;
    const int SCR_HEIGHT = 600;

    unsigned int VAO;
    unsigned int lightVAO;
    // textures
    // unsigned int texture1;
    // unsigned int texture2;
    // unsigned int diffuseMap;
    // unsigned int specularMap;
    Texture* texture1;
    Texture* texture2;
    Texture* diffuseMap;
    Texture* specularMap;

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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // normal attribute
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        // texture attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // element buffer object (EBO) - for indexed vertices - less memory consumption for bigger models
        unsigned int EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        // lightning VAO
        glGenVertexArrays(1, &lightVAO);
        glBindVertexArray(lightVAO);
        // leaving the VBO the same as for the normal objects
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        
        // // unbind VAO etc
        // glBindVertexArray(0);
        
        // camera
        camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

        // textures
        //stbi_set_flip_vertically_on_load(true);
        texture1 = new Texture("src/model/assets/container.jpg", false);
        texture2 = new Texture("src/model/assets/container.jpg", false);
        diffuseMap = new Texture("src/model/assets/container2.png", true);
        specularMap = new Texture("src/model/assets/container2_specular.png", true);

        birdTexture = new Texture("src/model/assets/bird/diffuse.jpg", false);
        backpackTexture = new Texture("src/model/assets/backpack/diffuse.jpg", false);
        penguinTexture = new Texture("src/model/assets/penguin/PenguinDiffuseColor.png", true);
        knightTexture = new Texture("src/model/assets/knight/armor.jpg", false);

        // downloaded models
        backpackModel = new Model("src/model/assets/backpack/backpack.obj", *backpackTexture);
        birdModel = new Model("src/model/assets/bird/bird.obj", *birdTexture);
        penguinModel = new Model("src/model/assets/penguin/PenguinBaseMesh.obj", *penguinTexture);
        knightModel = new Model("src/model/assets/knight/knight.obj", *knightTexture);
        //setTexture(birdTexture, "src/model/assets/bird/diffuse.jpg", false);
        
        ourShader->use();
        ourShader->setInt("texture1", 0);
        ourShader->setInt("texture2", 1);
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
        ourShader->setVec3("dirLight.direction", camera->Front);
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
        ourShader->setVec3("spotLight.position", camera->Position);
        ourShader->setVec3("spotLight.direction", camera->Front);
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

        ourShader->setVec3("viewPos", camera->Position);
       

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
            camera->ProcessKeyboard(FORWARD);
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera->ProcessKeyboard(BACKWARD);
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera->ProcessKeyboard(LEFT);
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera->ProcessKeyboard(RIGHT);
        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera->ProcessKeyboard(UP);
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera->ProcessKeyboard(DOWN);
        if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
            camera->ProcessMouseScroll(true);
        if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
            camera->ProcessMouseScroll(false);
    }

    void render() {
        // clear the colorbuffer and zbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        // activate normal shader
        // projection/view
        ourShader->use();
        glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera->GetViewMatrix();
        ourShader->setMat4("projection", projection);
        ourShader->setMat4("view", view);

        // activate texture (diffuse and specular map)
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, diffuseMap->ID);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, specularMap->ID);

        // transform and draw boxes
        glBindVertexArray(VAO);
        for(unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i; 
            //  static rotation
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            // rotation with time
            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

            ourShader->setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //draw downloaded model
        modelShader->use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, backpackTexture->ID);

        lightningShader->setMat4("projection", projection);
        lightningShader->setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(15.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        modelShader->setMat4("model", model);
        backpackModel->Draw(*modelShader);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, birdTexture->ID);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));	// it's a bit too big for our scene, so scale it down
        modelShader->setMat4("model", model);
        birdModel->Draw(*modelShader);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, penguinTexture->ID);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(20.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        modelShader->setMat4("model", model);
        penguinModel->Draw(*modelShader);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, knightTexture->ID);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));	// it's a bit too big for our scene, so scale it down
        modelShader->setMat4("model", model);
        knightModel->Draw(*modelShader);


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

    // // texture
    // void setTexture(unsigned int &texture, std::string sourceFile, bool isPNG) {
    //     glGenTextures(1, &texture);
    //     glBindTexture(GL_TEXTURE_2D, texture);  
        
    //     // set the texture wrapping
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //     // set the texture filtering options
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //     // load and generate the texture
    //     int width, height, nrChannels;
    //     unsigned char *data = stbi_load(sourceFile.c_str(), &width, &height, &nrChannels, 0);
    //     if(data) {
    //         if(isPNG) {
    //             glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //         }
    //         else {
    //             glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //         }
    //         glGenerateMipmap(GL_TEXTURE_2D);
    //     }
    //     else {
    //         std::cout << "Failed to load texture" << std::endl;
    //     }
        
    //     // free image memory
    //     stbi_image_free(data);
    // }


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

        camera->ProcessMouseMovement(xoffset, yoffset);
    }
};