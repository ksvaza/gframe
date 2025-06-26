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

    glm::mat4 GetMatrix() const
    {
        glm::mat4 matrix = glm::mat4(1.0f);

        matrix = glm::translate(matrix, position);

        matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));

        matrix = glm::scale(matrix, scale);

        return matrix;
    }

    EntityID parent = INVALID_ENTITY;
    std::vector<EntityID> children;
};