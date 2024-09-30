#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <utility>
#include "Model.cpp"
#include "Light.cpp"
#include "Shader.cpp"
#include "Camera.cpp"

class Renderer {
private:
    float screenHeight;
    float screenWidth;

public:
    Renderer(float screenWidth, float screenHeight) {
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;
    }

    void render(std::vector<std::pair<Shader*, std::vector<Model*>*>*>* toRender, std::vector<Light*>* lights, Camera* camera) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        // projection/view matrices
        glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), this->screenWidth / this->screenHeight, 0.1f, 100.0f);
        glm::mat4 view = camera->GetViewMatrix();
        
        for(auto obj : *toRender) {
            Shader* shader = obj->first;
            shader->use();
            shader->setMat4("projection", projection);
            shader->setMat4("view", view);
            for(Model* model : *obj->second) {
                glm::mat4 m = model->getModelMatrix();
                shader->setMat4("model", m);
                model->draw();
            }
        }


        // lamps
        // lightningShader->use();
        // lightningShader->setMat4("projection", projection);
        // lightningShader->setMat4("view", view);
        // glBindVertexArray(lightVAO);
        // for(unsigned int i = 0; i < 4; i++) {
        //     glm::mat4 model = glm::mat4(1.0f);
        //     model = glm::translate(model, pointLightPositions[i]);
        //     //model = glm::scale(glm::vec3(0.5f, 0.5f, 0.5f));
            
        //     lightningShader->setMat4("model", model);
        //     glDrawArrays(GL_TRIANGLES, 0, 36);
        // }
    }
};