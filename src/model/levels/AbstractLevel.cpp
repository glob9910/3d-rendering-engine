#include <vector>

#include "../Window.cpp"
#include "../models/Model.cpp"
#include "../lights/Light.cpp"
#include "../lights/DirLight.cpp"
#include "../lights/PointLight.cpp"
#include "../models/Box.cpp"
#include "../Shader.cpp"
#include "../models/LoadedModel.cpp"
#include "../Renderer.cpp"
#include "../Camera.cpp"

class AbstractLevel{
protected:

    std::vector<Light*>* lights;

    std::vector<Model*>* modelsForLightShader;
    std::vector<Model*>* modelsForModelShader;
    std::vector<std::pair<Shader*, std::vector<Model*>*>*>* toRender;

    Shader* skyboxShader;
    Shader* lightShader;
    Shader* modelShader;

    GLFWwindow *window;

    Renderer* renderer;
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
        glfwSetWindowUserPointer(window, this);

        camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));        
    }

    void render() {}

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
            camera->processMouseScroll(true);
        if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
            camera->processMouseScroll(false);

        if(detectCollisions) {
            for(Model* model : *modelsForModelShader) {
                camera->isCollision = detectCameraCollision(model);
                if(camera->isCollision) {
                    camera->position = lastPosition;
                    break;
                }
            }
        }
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
        // Obliczamy rzeczywisty rozmiar modelu w przestrzeni świata, biorąc pod uwagę jego pozycję i skalę
        glm::vec3 modelMin = model->position - model->scale;
        glm::vec3 modelMax = model->position + model->scale;

        // Pozycja kamery
        glm::vec3 cameraPosition = camera->position;

        // Sprawdzamy, czy pozycja kamery znajduje się w zakresie modelu na każdej osi
        bool collisionX = cameraPosition.x >= modelMin.x && cameraPosition.x <= modelMax.x;
        bool collisionY = cameraPosition.y >= modelMin.y && cameraPosition.y <= modelMax.y;
        bool collisionZ = cameraPosition.z >= modelMin.z && cameraPosition.z <= modelMax.z;

        // Jeśli pozycja kamery znajduje się w modelu na wszystkich osiach, to mamy kolizję
        if (collisionX && collisionY && collisionZ)
            printf("collision\n");

        return collisionX && collisionY && collisionZ;
    }
};