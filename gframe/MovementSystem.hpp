#pragma once
#include "ecs.hpp"
#include "InputTag.hpp"
#include "Velocity.hpp"
#include <glm/trigonometric.hpp>
#include "InputManager.hpp"
#include "PlayerTag.hpp"
#include "GframeData.hpp"
#include "TransformComponent.hpp"

inline void MovementSystem(void* context, float dt) 
{
    auto* data = static_cast<GframeData*>(context);
    auto& ecs = data->ecs;
    auto archetypes = ecs.FindArchetypesWithMask<InputTag, TransformComponent, Velocity>();

    for (ECS::Archetype* arch : archetypes)
    {
        if (arch->hasComponents<PlayerTag>())
        {
            size_t count = arch->GetEntityCount();

            for (size_t i = 0; i < count; i++)
            {
                auto* orient = arch->GetComponent<TransformComponent>(i);
                auto* vel = arch->GetComponent<Velocity>(i);

                if (InputManager::HWInputs::Mouse.MouseButton[0])
                {
                    orient->rotation.y += InputManager::HWInputs::Mouse.MouseDeltaPosition.x * 0.3;
                    orient->rotation.x -= InputManager::HWInputs::Mouse.MouseDeltaPosition.y * 0.3;

                    if (orient->rotation.x > 89.0f) orient->rotation.x = 89.0f;
                    if (orient->rotation.x < -89.0f) orient->rotation.x = -89.0f;

                    glfwSetInputMode(data->window.glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                }
                else
                    glfwSetInputMode(data->window.glWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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



                float speed = 35.0f;

                float yaw = glm::radians(orient->rotation.y);
                float pitch = glm::radians(orient->rotation.x);

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
}
