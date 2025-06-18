#pragma once
#include "Types.hpp"
#include "Archetype.hpp"
#include <functional>
#include <iostream>

namespace ECS
{
    class ecs
    {
    public:
        using SystemFn = std::function<void(ecs&, float)>;
        EntityID CreateEntity();
        
        void RemoveEntity(EntityID entity);
        
        template<typename T>
        void AddComponent(EntityID entity, T&& component);
        
        template<typename T>
        void RemoveComponent(EntityID entity);

        template<typename T>
        ComponentID RegisterComponent();
        
        std::unordered_map<ComponentID, void*> GetAllComponentsForEntity(EntityID entity);

        template<typename T>
        T* GetComponentForEntity(EntityID entity);

        void* GetComponentForEntity(EntityID entity, ComponentID id);

        Archetype* FindArchetypeWithExactMask(const mask& m) const;
        
        std::vector<Archetype*> FindArchetypesWithMask(const mask& m) const;
        
        template<typename... T>
        std::vector<Archetype*> FindArchetypesWithMask() const;
        
        void RegisterSystem(SystemFn fn);
        
        void UpdateSystems(float dt);
        
        template<typename T>
        ComponentID GetComponentId();
        
    private:
        EntityID NextId = 0;
        ComponentFactory factory;
        std::unordered_map<mask, std::unique_ptr<Archetype>> archetypes;
        std::unordered_map<EntityID, Archetype*> entityToArchetype;
        std::vector<SystemFn> systems;
    };

    template<typename T>
    inline void ecs::AddComponent(EntityID entity, T&& component)
    {
        using CleanT = std::decay_t<T>;
        ComponentID id = factory.GetId<T>();

        auto it = entityToArchetype.find(entity);
        mask NewMask;

        NewMask.set(id);
        std::unordered_map<ComponentID, void*> componentData;
        componentData[id] = new CleanT(std::forward<T>(component));

        if (it == entityToArchetype.end())
        {
            auto newArch = std::make_unique<Archetype>(NewMask);
            newArch->AddEntity(entity, componentData);
            entityToArchetype[entity] = newArch.get();
            archetypes[NewMask] = std::move(newArch);
            return;
        }

        Archetype* arch = it->second;
        componentData = arch->GetAllComponentsForEntity(entity);
        componentData[id] = new CleanT(std::forward<T>(component));

        mask newMask = arch->GetSignature();
        newMask.set(id);

        auto it2 = archetypes.find(newMask);
        if (it2 == archetypes.end())
        {
            auto newArch = std::make_unique<Archetype>(newMask);
            newArch->AddEntity(entity, componentData);
            arch->RemoveEntity(entity);
            entityToArchetype[entity] = newArch.get();
            archetypes[newMask] = std::move(newArch);
            return;
        }

        Archetype* newArch = it2->second.get();
        newArch->AddEntity(entity, componentData);
        arch->RemoveEntity(entity);
        entityToArchetype[entity] = newArch;
    }

    template<typename T>
    inline void ecs::RemoveComponent(EntityID entity)
    {
        auto it = entityToArchetype.find(entity);
        if (it == entityToArchetype.end())
        {
            std::cout << "no Entity to remove Component from!\n";
            return;
        }

        Archetype* arch = it->second;
        ComponentID id = factory.GetId<T>();
        std::unordered_map<ComponentID, void*> componentData = arch->GetAllComponentsForEntity(entity);
        componentData.erase(id);
        mask NewMask = arch->GetSignature();
        NewMask[id] = 0;
        auto it2 = archetypes.find(NewMask);
        if (it2 == archetypes.end())
        {
            auto newArch = std::make_unique<Archetype>(NewMask);
            newArch->AddEntity(entity, componentData);
            arch->RemoveEntity(entity);
            entityToArchetype[entity] = newArch.get();
            archetypes[NewMask] = std::move(newArch);
            return;
        }
        Archetype* newArch = it2->second.get();
        newArch->AddEntity(entity, componentData);
        arch->RemoveEntity(entity);
        entityToArchetype[entity] = newArch;

    }

    template<typename T>
    inline ComponentID ecs::RegisterComponent()
    {
        return factory.RegisterComponent<T>();
    }

    template<typename T>
    inline T* ecs::GetComponentForEntity(EntityID entity)
    {
        auto it = entityToArchetype.find(entity);
        if (it == entityToArchetype.end()) 
        {
            throw std::runtime_error("Entity not found");
        }

        Archetype* arch = it->second;
        ComponentID id = factory.GetId<T>();
        void* component = arch->GetComponentForEntity(entity, id);
        if (!component)
        {
            throw std::runtime_error("Component not found for entity");
        }

        return static_cast<T*>(component);
    }

    template<typename ...T>
    inline std::vector<Archetype*> ecs::FindArchetypesWithMask() const
    {
        mask m;
        (m.set(factory.GetId<T>()), ...);
        std::vector<Archetype*> result;

        for (const auto& [archMask, archetype] : archetypes) 
        {
            if ((archMask & m) == m) 
            {
                result.push_back(archetype.get());
            }
        }

        return result;
    }

    template<typename T>
    inline ComponentID ecs::GetComponentId()
    {
        return factory.GetId<T>();
    }
}