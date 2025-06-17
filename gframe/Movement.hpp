#pragma once
#include "ecs.hpp"
#include "Velocity.hpp"
#include "Position.hpp"

void UpdateMovement(ECS::ecs& _ecs, float dt)
{
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