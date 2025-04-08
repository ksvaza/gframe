#pragma once
#include <glm/glm.hpp>

class Transform
{
public:
    glm::vec3 position = glm::vec3(0.0);
    glm::vec3 rotation = glm::vec3(0.0);
    glm::vec3 scale = glm::vec3(0.0);
    Transform();
    Transform(glm::vec3 position);
    Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
};
