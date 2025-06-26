#pragma once
#include "ecs.hpp"
#include "GframeData.hpp"
#include "TransformComponent.hpp"

inline void UpdateMovement(void* context, float dt)
{
    auto* data = static_cast<GframeData*>(context);
    auto& _ecs = data->ecs;
    auto archetypes = _ecs.FindArchetypesWithMask<TransformComponent, Velocity>();

    for (ECS::Archetype* arch : archetypes)
    {
        size_t entityCount = arch->GetEntityCount();

        for (size_t i = 0; i < entityCount; i++)
        {
            auto* transform = arch->GetComponent<TransformComponent>(i);
            auto* vel = arch->GetComponent<Velocity>(i);

            transform->position.x += vel->vx * dt;
            transform->position.y += vel->vy * dt;
            transform->position.z += vel->vz * dt;
        }
    }
}