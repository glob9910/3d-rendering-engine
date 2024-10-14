#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  0.3f;
const float SENSITIVITY =  0.3f;
const float ZOOM        =  45.0f;
const float ZOOM_SENSITIVITY = 1.0f;

class Camera {
public:

    // camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    // euler Angles
    float yaw;
    float pitch;
    // camera options
    float movementSpeed;
    float mouseSensitivity;
    float zoom;
    bool isCollision;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
        this->position = position;
        this->worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        updateCameraVectors();
        isCollision = false;
    }

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(position, position + front, up);
    }

    void processKeyboard(Camera_Movement direction) {
        float velocity = movementSpeed;
        if (direction == FORWARD)
            position += front * velocity;
        if (direction == BACKWARD)
            position -= front * velocity;
        if (direction == LEFT)
            position -= right * velocity;
        if (direction == RIGHT)
            position += right * velocity;
        if (direction == UP)
            position.y += velocity;
        if (direction == DOWN)
            position.y -= velocity;
    }

    void processMouseMovement(float xoffset, float yoffset) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw   += xoffset;
        pitch += yoffset;

        // when pitch is out of bounds, screen doesn't get flipped
        if (pitch > 89.0f)
           pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;        

        updateCameraVectors();
    }

    void processZoom(bool zoom) {
        if(zoom)
            this->zoom -= ZOOM_SENSITIVITY;
        else 
            this->zoom += ZOOM_SENSITIVITY;

        if (this->zoom < 1.0f)
            this->zoom = 1.0f;
        if (this->zoom > 45.0f)
            this->zoom = 45.0f;
    }

private:
    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->front = glm::normalize(front);

        right = glm::normalize(glm::cross(this->front, worldUp));
        up = glm::normalize(glm::cross(right, this->front));
    }
};
