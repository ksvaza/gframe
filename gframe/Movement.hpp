#pragma once
#include "ecs.hpp"
#include "Velocity.hpp"
#include "Position.hpp"
#include "GframeData.hpp"

inline void UpdateMovement(void* context, float dt)
{
    auto* data = static_cast<GframeData*>(context);
    auto& _ecs = data->ecs;
    auto archetypes = _ecs.FindArchetypesWithMask<Position, Velocity>();

    for (ECS::Archetype* arch : archetypes)
    {
        size_t entityCount = arch->GetEntityCount();

        for (size_t i = 0; i < entityCount; i++)
        {
            auto* pos = arch->GetComponent<Position>(i);
            auto* vel = arch->GetComponent<Velocity>(i);

            pos->x += vel->vx * dt;
            pos->y += vel->vy * dt;
            pos->z += vel->vz * dt;
        }
    }
}