#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

class Camera
{
public:
    glm::vec3 position;
    glm::vec3 rotation = { 0.0f, 1.0f, 0.0f };

    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    Camera() = default;

    glm::mat4 GetViewMatrix() const;

    glm::mat4 GetProjectionMatrix() const;

    glm::vec3 GetForward() const;

    glm::vec3 GetRight() const;
    
    glm::vec3 GetUp() const;
};
