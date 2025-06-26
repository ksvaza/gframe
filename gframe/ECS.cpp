#include "ecs.hpp"

EntityID ECS::ecs::CreateEntity()
{
    return NextId++;
}

void ECS::ecs::RemoveEntity(EntityID entity)
{
    auto it = entityToArchetype.find(entity);
    if (it == entityToArchetype.end())
    {
        std::cout << "no entity to remove\n";
    }
    it->second->RemoveEntity(entity);
}

std::unordered_map<ComponentID, void*> ECS::ecs::GetAllComponentsForEntity(EntityID entity)
{
    auto it = entityToArchetype.find(entity);
    if (it == entityToArchetype.end()) {
        throw std::runtime_error("Entity not found");
    }

    Archetype* arch = it->second;
    return arch->GetAllComponentsForEntity(entity);
}

void* ECS::ecs::GetComponentForEntity(EntityID entity, ComponentID id)
{
    auto it = entityToArchetype.find(entity);
    if (it == entityToArchetype.end())
    {
        throw std::runtime_error("Entity not found");
    }

    Archetype* arch = it->second;
    void* component = arch->GetComponentForEntity(entity, id);
    if (!component)
    {
        throw std::runtime_error("Component not found for entity");
    }
    return component;
}

ECS::Archetype* ECS::ecs::FindArchetypeWithExactMask(const mask& m) const
{
    auto it = archetypes.find(m);
    if (it != archetypes.end()) {
        return it->second.get();
    }
    return nullptr;
}

std::vector<ECS::Archetype*> ECS::ecs::FindArchetypesWithMask(const mask& m) const
{
    std::vector<Archetype*> result;

    for (const auto& [archMask, archetype] : archetypes) {
        if ((archMask & m) == m) {
            result.push_back(archetype.get());
        }
    }

    return result;
}

void ECS::ecs::RegisterSystem(SystemFn fn)
{
    systems.push_back(std::move(fn));
}

void ECS::ecs::UpdateSystems(void* context,float dt)
{
    for (auto& sys : systems)
    {
        sys(context, dt);
    }
}
