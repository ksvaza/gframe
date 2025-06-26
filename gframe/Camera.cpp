#include "Camera.hpp"

glm::mat4 Camera::GetViewMatrix() const
{
    glm::vec3 forward = GetForward();
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); //ja grib Space engineers roll tad up ir janomaina uz cameraUp(un tads jauztaisa)

    return glm::lookAt(position, position + forward, up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

glm::vec3 Camera::GetForward() const
{
    float yaw = glm::radians(rotation.y);
    float pitch = glm::radians(rotation.x);

    return glm::normalize(glm::vec3(
        cos(pitch) * cos(yaw),
        sin(pitch),
        cos(pitch) * sin(yaw)
    ));
}

glm::vec3 Camera::GetRight() const
{
    glm::vec3 forward = GetForward();
    return glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
}

glm::vec3 Camera::GetUp() const
{
    glm::vec3 forward = GetForward();
    glm::vec3 right = GetRight();
    return glm::normalize(glm::cross(right, forward));
}


//!!
//The rotation vector is expected to be in radians for the GetForward and GetUp
//!!