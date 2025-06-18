#pragma once
#include "ecs.hpp"
#include "InputTag.hpp"
#include "Orientation.hpp"
#include "Velocity.hpp"
#include <glm/trigonometric.hpp>
#include "InputManager.hpp"
#include "PlayerTag.hpp"

inline void MovementSystem(ECS::ecs& ecs, float dt) {

   /* auto archetypes = ecs.FindArchetypesWithMask<InputTag, Orientation, Velocity>();

    for (ECS::Archetype* arch : archetypes)
    {
        if (arch->hasComponents<PlayerTag>())
        {
            size_t count = arch->GetEntityCount();

            for (size_t i = 0; i < count; i++)
            {
                auto* orient = arch->GetComponent<Orientation>(i);
                auto* vel = arch->GetComponent<Velocity>(i);

                if (InputManager::HWInputs::Mouse.MouseButton[0])
                {
                    orient->yaw += InputManager::HWInputs::Mouse.MouseDeltaPosition.x * 0.1;
                    orient->pitch -= InputManager::HWInputs::Mouse.MouseDeltaPosition.y * 0.1;
                }

                float forwardInput = 0.0f;
                float rightInput = 0.0f;
                float upInput = 0.0f;


                if (InputManager::HWInputs::Keyboard.Key[GLFW_KEY_W])
                    forwardInput = 1;
                if (InputManager::HWInputs::Keyboard.Key[GLFW_KEY_S])
                    forwardInput = -1;
                if (InputManager::HWInputs::Keyboard.Key[GLFW_KEY_A])
                    rightInput = -1;
                if (InputManager::HWInputs::Keyboard.Key[GLFW_KEY_D])
                    rightInput = 1;
                if (InputManager::HWInputs::Keyboard.Key[GLFW_KEY_LEFT_SHIFT])
                    upInput = -1;
                if (InputManager::HWInputs::Keyboard.Key[GLFW_KEY_SPACE])
                    upInput = 1;



                float speed = 25.0f;

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
    }*/
}
