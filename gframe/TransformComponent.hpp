#pragma once
#include <glm/glm.hpp>
#include "Types.hpp"
#include <vector>

class TransformComponent
{
public:
    glm::vec3 position = glm::vec3(0.0);
    glm::vec3 rotation = glm::vec3(0.0);// x - pitch, y - yaw, z - roll
    glm::vec3 scale = glm::vec3(1.0);

    TransformComponent() = default;
    TransformComponent(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
        : position(position), rotation(rotation), scale(scale) {};
    TransformComponent(glm::vec3 position)
        : position(position), rotation(glm::vec3(0.0)), scale(glm::vec3(1.0)) {};

    EntityID parent = INVALID_ENTITY;
    std::vector<EntityID> children;
};