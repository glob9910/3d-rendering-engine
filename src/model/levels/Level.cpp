#include "AbstractLevel.cpp"

class Level : public AbstractLevel {
private:
    std::vector<Model*>* modelsForLightShader;
    std::vector<Model*>* modelsForModelShader;

public:
    Level(int SCR_WIDTH, int SCR_HEIGHT, GLFWwindow *window) : AbstractLevel(SCR_WIDTH, SCR_HEIGHT, window) {
        Shader* modelShader = new Shader("src/model/shaders/shader.vs", "src/model/shaders/shader.fs");
        Shader* lightShader = new Shader("src/model/shaders/lightShader.vs", "src/model/shaders/lightShader.fs");
        Shader* skyboxShader = new Shader("src/model/shaders/skyboxShader.vs", "src/model/shaders/skyboxShader.fs");

        lights = new std::vector<Light*>();
        modelsForLightShader = new std::vector<Model*>();
        modelsForModelShader = new std::vector<Model*>();

        createDirLight();
        createPointLights();
        createModels();
        createBoxes();
        skybox = new Skybox("src/model/assets/skybox", skyboxShader);
        collisableModels = modelsForModelShader;

        std::pair<Shader*, std::vector<Model*>*>* lightShaderModels = new std::pair<Shader*, std::vector<Model*>*>(lightShader, modelsForLightShader);
        std::pair<Shader*, std::vector<Model*>*>* modelShaderModels = new std::pair<Shader*, std::vector<Model*>*>(modelShader, modelsForModelShader);

        renderer = new Renderer(SCR_WIDTH, SCR_HEIGHT);
        toRender = new std::vector<std::pair<Shader*, std::vector<Model*>*>*>();
        toRender->push_back(lightShaderModels);
        toRender->push_back(modelShaderModels);
        toRender->push_back(modelShaderModels);
    }

    void render() {
        // collisions
        for(Model* model : *modelsForModelShader) {
            camera->isCollision = detectCameraCollision(model);
        }

        // pinguin rotation
        modelsForModelShader->at(2)->rotationAngle = (float)glfwGetTime();
        modelsForModelShader->at(2)->rotation = glm::vec3(0.0f, 1.0f, 0.0f);

        processAction(window);
        renderer->render(toRender, lights, camera, skybox);
    }

private:
    void createDirLight() {
        DirLight* dirLight = new DirLight();
        dirLight->setAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
        dirLight->setDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));
        dirLight->setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
        dirLight->setDirection(glm::vec3(0.0f, -1000.0f, 0.0f));
        lights->push_back(dirLight);
    }

    void createPointLights() {
        std::vector<PointLight*>* pointLights = new std::vector<PointLight*>();
        pointLights->push_back(createPointLight(glm::vec3(0.0f, 0.0f, 0.0f)));
        pointLights->push_back(createPointLight(glm::vec3(1.0f, 1.0f, 1.0f)));

        for(PointLight* pointLight : *pointLights) {
            modelsForLightShader->push_back(pointLight->getModel());
            lights->push_back(pointLight);
        }
    }

    PointLight* createPointLight(glm::vec3 position) {
        PointLight* pointLight = new PointLight();
        pointLight->setPosition(position);
        pointLight->setModel(createBox(pointLight->getPosition()));
        pointLight->model->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
        pointLight->setAmbient(glm::vec3(0.0f, 0.0f, 0.0f));
        pointLight->setDiffuse(glm::vec3(0.8f, 0.8f, 0.8f));
        pointLight->setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
        pointLight->setConstant(1.0f);
        pointLight->setLinear(0.09f);
        pointLight->setQuadratic(0.032f);   
        return pointLight;    
    }

    Box* createBox(glm::vec3 position) {
        Box* box = new Box();
        box->setPosition(position);
        box->setMaterial(new Material(
            new Texture("src/model/assets/container2.png", true),
            new Texture("src/model/assets/container2_specular.png", true),
            32.0f
        ));
        return box;
    }

    void createModels() {
        //Model* backpack = new LoadedModel("src/model/assets/backpack/backpack.obj", new Texture("src/model/assets/backpack/diffuse.jpg", false));
        Model* bird = new LoadedModel("src/model/assets/bird/bird_normalized.obj", new Texture("src/model/assets/bird/diffuse.jpg", false));
        Model* knight = new LoadedModel("src/model/assets/knight/knight2.obj", new Texture("src/model/assets/knight/armor.jpg", false));
        Model* penguin = new LoadedModel("src/model/assets/penguin/penguin.obj", new Texture("src/model/assets/penguin/PenguinDiffuseColor.png", true));

        //backpack->setPosition(glm::vec3(6, 0, 0));
        bird->setPosition(glm::vec3(3.0f, 0.0f, 0.0f));
        knight->setPosition(glm::vec3(-8.0f, 0.0f ,0.0f));
        penguin->setPosition(glm::vec3(-1.0f, 0.0f, 0.0f));

        //modelsForModelShader->push_back(backpack);
        modelsForModelShader->push_back(bird);
        modelsForModelShader->push_back(knight);
        modelsForModelShader->push_back(penguin);
    }

    void createBoxes() {
        modelsForModelShader->push_back(createBox(glm::vec3( 2.0f,  5.0f, -15.0f)));
        modelsForModelShader->push_back(createBox(glm::vec3(-1.5f, -2.2f, -2.5f)));
        modelsForModelShader->push_back(createBox(glm::vec3(-3.8f, -2.0f, -12.3f)));
        modelsForModelShader->push_back(createBox(glm::vec3( 2.4f, -0.4f, -3.5f)));
        modelsForModelShader->push_back(createBox(glm::vec3(-1.7f,  3.0f, -7.5f)));
        modelsForModelShader->push_back(createBox(glm::vec3( 1.3f, -2.0f, -2.5f)));
        modelsForModelShader->push_back(createBox(glm::vec3( 1.5f,  2.0f, -2.5f)));
        modelsForModelShader->push_back(createBox(glm::vec3( 1.5f,  0.2f, -1.5f)));
    }

    void processAction(GLFWwindow *window) {
        const float speed = 0.5f;
        PointLight* light = (PointLight*)lights->at(1);

        if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
            light->position.x -= speed;
            light->model->position.x -= speed;
        }
        if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
            light->position.x += speed;
            light->model->position.x += speed;
        }
        if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
            light->position.y -= speed;
            light->model->position.y -= speed;
        }
        if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
            light->position.y += speed;
            light->model->position.y += speed;
        }
        if(glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
            light->position.z -= speed;
            light->model->position.z -= speed;
        }
        if(glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
            light->position.z += speed;
            light->model->position.z += speed;
        }
    }
};
