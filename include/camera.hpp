#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
public:
 enum DIRECTION 
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

public:
    Camera(const glm::vec3& pos = glm::vec3(0.0f, 0.0f, 0.0f));

public:
    const glm::mat4 GetView() const { return glm::lookAt(position, position + front, up); }

    void ProcessKeyboard(const DIRECTION& direction, const float& deltaTime);
    void ProcessMouse(const float& xpos, const float& ypos);

    void SetSpeed(const float& speed) { this->speed = speed; }
    void SetSensitivity(const float& sensitiviti) { this->sensitivity = sensitivity; }

private:
    void update();   

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw         = -90.0f;
    float pitch       = 0.0f;
    float speed       = 130.0f;
    float sensitivity = 0.1f;
    float zoom        = 45.0f;

    float lastX;
    float lastY;
    bool  firstMouse = true;
  
};