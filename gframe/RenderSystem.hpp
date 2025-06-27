#pragma once
#include "ecs.hpp"
#include "GframeData.hpp"
#include "TransformComponent.hpp"
#include "RenderTag.hpp"
#include "MeshComponent.hpp"

inline void RenderSystem(void* context, float dt)
{
    auto* data = static_cast<GframeData*>(context);
    auto& _ecs = data->ecs;

    auto archetypes = _ecs.FindArchetypesWithMask<TransformComponent, RenderTag, MeshComponent>();

    for (ECS::Archetype* arch : archetypes)
    {
        size_t entityCount = arch->GetEntityCount();

        for (size_t i = 0; i < entityCount; i++)
        {
            auto* transform = arch->GetComponent<TransformComponent>(i);
            auto* mesh = arch->GetComponent<MeshComponent>(i);
            auto* Renderer = arch->GetComponent<RenderTag>(i);
            
            data->Render.EcsDrawMesh(*mesh, *transform, data->AssetManager.GetShader(Renderer->ShaderKey), data->camera);
        }
    }
}