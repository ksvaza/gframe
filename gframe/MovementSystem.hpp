#pragma once
#include "ecs.hpp"
#include "InputTag.hpp"
#include "Orientation.hpp"
#include "Velocity.hpp"
#include <glm/trigonometric.hpp>

inline void MovementSystem(ECS::ecs& ecs, float dt) {
    auto archetypes = ecs.FindArchetypesWithMask<InputTag, Orientation, Velocity>();

    for (ECS::Archetype* arch : archetypes) {
        size_t count = arch->GetEntityCount();

        for (size_t i = 0; i < count; ++i) {
            auto* orient = arch->GetComponent<Orientation>(i);
            auto* vel = arch->GetComponent<Velocity>(i);

            

            // Access input from global InputManager
            float forwardInput = /* InputManager::GetAxis("forward") */ 1.0f;
            float rightInput = /* InputManager::GetAxis("right") */ 0.0f;
            float upInput = /* InputManager::GetAxis("up") */ 0.0f;

            // Access movement speed
            float speed = /* InputManager::GetMovementSpeed() or constant */ 5.0f;

            float yaw = glm::radians(orient->yaw);
            float pitch = glm::radians(orient->pitch);

            glm::vec3 forward = glm::normalize(glm::vec3(
                cos(yaw) * cos(pitch),
                sin(pitch),
                sin(yaw) * cos(pitch)
            ));

            glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
            glm::vec3 up = glm::normalize(glm::cross(right, forward));

            glm::vec3 direction = forward * forwardInput + right * rightInput + up * upInput;
            direction *= speed;

            vel->vx = direction.x;
            vel->vy = direction.y;
            vel->vz = direction.z;
        }
    }
}
