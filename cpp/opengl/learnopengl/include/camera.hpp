#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

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

    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW,
        float pitch = PITCH
    ) :
        front(glm::vec3(0.0f, 0.0f, -1.0f)),
        movementSpeed(SPEED),
        mouseSensitivity(SENSITIVITY),
        zoom(ZOOM)
    {
        this->position = position;
        this->worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;

        this->updateCameraVectors();
    }

    Camera(
        float posX,
        float posY,
        float posZ,
        float upX,
        float upY,
        float upZ,
        float yaw,
        float pitch
    ) :
        front(glm::vec3(0.0f, 0.0f, -1.0f)),
        movementSpeed(SPEED),
        mouseSensitivity(SENSITIVITY),
        zoom(ZOOM)
    {
        this->position = glm::vec3(posX, posY, posZ);
        this->worldUp = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;

        this->updateCameraVectors();
    }

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(this->position, this->position + this->front, this->up);
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = this->movementSpeed * deltaTime;

        if (direction == FORWARD)
            this->position += this->front * velocity;

        if (direction == BACKWARD)
            this->position -= this->front * velocity;

        if (direction == LEFT)
            this->position -= this->right * velocity;

        if (direction == RIGHT)
            this->position += this->right * velocity;
    }

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
        xoffset *= this->mouseSensitivity;
        yoffset *= this->mouseSensitivity;

        this->yaw   += xoffset;
        this->pitch += yoffset;

        if (constrainPitch) {
            if (this->pitch >  89.0f) this->pitch =  89.0f;
            if (this->pitch < -89.0f) this->pitch = -89.0f;
        }

        this->updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset) {
        this->zoom -= (float)yoffset;

        if (this->zoom < 1.0f)  this->zoom = 1.0f;
        if (this->zoom > 45.0f) this->zoom = 45.0f;
    }

private:
    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        front.y = sin(glm::radians(this->pitch));
        front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        this->front = glm::normalize(front);

        this->right = glm::normalize(glm::cross(this->front, this->worldUp));
        this->up    = glm::normalize(glm::cross(this->right, this->front));
    }
};

#endif // CAMERA_H
