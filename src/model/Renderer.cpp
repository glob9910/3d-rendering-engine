#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <vector>
#include <utility>

#include "models/Model.cpp"
#include "lights/Light.cpp"
#include "lights/DirLight.cpp"
#include "lights/PointLight.cpp"
#include "Shader.cpp"
#include "Camera.cpp"
#include "models/Material.cpp"
#include "models/Skybox.cpp"
#include "levels/AbstractLevel.cpp"

class Renderer {
private:
    float screenHeight;
    float screenWidth;

    glm::mat4 projection;
    glm::mat4 view;

public:
    Renderer(float screenWidth, float screenHeight) {
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;
    }

    void render(AbstractLevel* level) {
        std::vector<std::pair<Shader*, std::vector<Model*>*>*>* toRender = level->getModels();
        std::vector<Light*>* lights = level->getLights();
        Camera* camera = level->getCamera();
        Skybox* skybox = level->getSkybox();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // projection/view matrices
        projection = glm::perspective(glm::radians(camera->zoom), this->screenWidth / this->screenHeight, 0.1f, 100.0f);
        view = camera->GetViewMatrix();

        renderSkybox(skybox);
        for(auto obj : *toRender) {
            Shader* shader = obj->first;
            shader->use();

            renderLights(lights, shader);
            renderModels(*obj->second, shader);
        }
    }

private:
    void renderSkybox(Skybox* skybox) {
        if(skybox == nullptr) {
            printf(":()");
            return;
        }

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL); // let skybox render in background
        skybox->shader->use();
        skybox->shader->setInt("skybox", 0);
        glm::mat4 v = glm::mat4(glm::mat3(this->view));  
        skybox->shader->setMat4("view", v);
        skybox->shader->setMat4("projection", projection);
        glBindVertexArray(skybox->VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glDepthFunc(GL_LESS);   // default depth
        glDepthMask(GL_TRUE);
    }

    void renderLights(std::vector<Light*>* lights, Shader* shader) {
        int countPointLights = 0;
        for(Light* l : *lights) {
            glm::vec3 ambient = l->getAmbient();
            glm::vec3 diffuse = l->getDiffuse();
            glm::vec3 specular = l->getSpecular();
            switch(l->getType()) {
                case LIGHT::DIR:
                    glm::vec3 direction;
                    direction = ((DirLight*) l)->getDirection();
                    shader->setVec3("dirLight.direction", direction);
                    shader->setVec3("dirLight.ambient", ambient);
                    shader->setVec3("dirLight.diffuse", diffuse);
                    shader->setVec3("dirLight.specular", specular);
                    break;
                case LIGHT::POINT:
                    glm::vec3 position;
                    position = ((PointLight*) l)->getPosition();
                    float constant, linear, quadratic;
                    constant = ((PointLight*) l)->getConstant();
                    linear = ((PointLight*) l)->getLinear();
                    quadratic =((PointLight*) l)->getQuadratic();
                    shader->setVec3("pointLight[" + std::to_string(countPointLights) + "].ambient", ambient);
                    shader->setVec3("pointLight[" + std::to_string(countPointLights) + "].diffuse", diffuse);
                    shader->setVec3("pointLight[" + std::to_string(countPointLights) + "].specular", specular);
                    shader->setVec3("pointLight[" + std::to_string(countPointLights) + "].position", position);
                    shader->setFloat("pointLight[" + std::to_string(countPointLights) + "].constant", constant);
                    shader->setFloat("pointLight[" + std::to_string(countPointLights) + "].linear", linear);
                    shader->setFloat("pointLight[" + std::to_string(countPointLights) + "].quadratic", quadratic);
                    countPointLights++;
                default:
                    break;
            }
        }

        shader->setInt("numOfPointLights", countPointLights);
    }
    

    void renderModels(std::vector<Model*> models, Shader* shader) {
        shader->setMat4("projection", projection);
        shader->setMat4("view", view);
        shader->setInt("material.diffuse", 0);
        shader->setInt("material.specular", 1);
        for(Model* model : models) {
            glm::mat4 m = model->getModelMatrix();
            shader->setMat4("model", m);
            
            Material* material = model->getMaterial();
            shader->setFloat("material.shininess", material->getShininess());
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, material->getDiffuse()->ID);
            if(material->hasSpecular()) {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, material->getSpecular()->ID);
            }

            std::vector<Mesh> meshes = model->getMeshes();
            for(unsigned int i = 0; i < meshes.size(); i++) {
                meshes[i].draw();
            }

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
};