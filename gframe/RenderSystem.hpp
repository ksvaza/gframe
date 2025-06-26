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


            data->Render.Clear(glm::vec4(0.8, 0.0, 0.0, 1.0));
            //std::cout << "shader ID: " << data->AssetManager.GetShader(Renderer->ShaderKey).GetID() << " | " << Renderer->ShaderKey << '\n';
            data->Render.EcsDrawMesh(*mesh, *transform, data->AssetManager.GetShader(Renderer->ShaderKey), data->camera);

            //output camera and transform
            //std::cout << "Camera Position: " << data->camera.position.x << ' ' << data->camera.position.y << ' ' << data->camera.position.z << '\n';
            //std::cout << "Transform Position: " << transform->position.x << ' ' << transform->position.y << ' ' << transform->position.z << '\n';
            //std::cout << "size: " << mesh->vertices.size() << '\n';
            //for (auto x : mesh->vertices)
            //    std::cout << x.x << ' ' << x.y << ' ' << x.z << " | ";
            //std::cout << '\n';
        }
    }
}