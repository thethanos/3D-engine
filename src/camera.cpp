#include "camera.hpp"

Camera::Camera(const glm::vec3& pos)
{   
    position = pos;
    worldUp  = glm::vec3(0.0f, 1.0f, 0.0f);
    update();
}

void Camera::ProcessKeyboard(const DIRECTION& direction, const float& deltaTime)
{
    float velocity = speed * deltaTime;
    
    switch (direction) 
    {
    case FORWARD:
        position += front * velocity;
        break;
    case BACKWARD:
        position -= front * velocity;
        break;
    case LEFT:
        position -= right * velocity;
        break;
    case RIGHT:
        position += right * velocity;
        break;
    }
}

void Camera::ProcessMouse(const float& xpos, const float& ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 

    lastX = xpos;
    lastY = ypos;

    yaw   += (xoffset * sensitivity);
    pitch += (yoffset * sensitivity);

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    update();
}

void Camera::update()
{
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, worldUp));
    up    = glm::normalize(glm::cross(right, front));
}