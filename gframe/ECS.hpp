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
        using SystemFn = std::function<void(void*, float)>;

        /// <summary>
        /// Returns new EntityId
        /// </summary>
        /// <returns></returns>
        EntityID CreateEntity();
        
        /// <summary>
        /// Removes entityId from archetypes
        /// </summary>
        /// <param name="entity"></param>
        void RemoveEntity(EntityID entity);
        
        /*template<typename T>
        void AddComponent(EntityID entity, T&& component);*/

        /// <summary>
        /// Add a component to entityId
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="entity"></param>
        template<typename T>
        void AddComponent(EntityID entity);

        /// <summary>
        /// Add a component with initial Data to EntityId
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="entity"></param>
        /// <param name="component"></param>
        template<typename T>
        void AddComponentData(EntityID entity, T component);
        
        /// <summary>
        /// Remove a component from entityId
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="entity"></param>
        template<typename T>
        void RemoveComponent(EntityID entity);

        /// <summary>
        /// Register Component
        /// If component is not registered it cant be used
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        template<typename T>
        ComponentID RegisterComponent();
        
        /// <summary>
        /// returns unordered_map of all components for entity
        /// </summary>
        /// <param name="entity"></param>
        /// <returns></returns>
        std::unordered_map<ComponentID, void*> GetAllComponentsForEntity(EntityID entity);

        /// <summary>
        /// Returns a pointer to the component of type T for the given entity.
        /// Use only if certain that entity has Component T
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="entity"></param>
        /// <returns></returns>
        template<typename T>
        T* GetComponentForEntity(EntityID entity);

        /// <summary>
        /// Returns a pointer to the component of id ComponentID for the given entity.
        /// Use only if certain that entity has Component with id
        /// </summary>
        /// <param name="entity"></param>
        /// <param name="id"></param>
        /// <returns></returns>
        void* GetComponentForEntity(EntityID entity, ComponentID id);

        /// <summary>
        /// Returns pointer to Archetype with mask exactly maching mask m
        /// </summary>
        /// <param name="m"></param>
        /// <returns></returns>
        Archetype* FindArchetypeWithExactMask(const mask& m) const;
        
        /// <summary>
        /// Returns vector of pointers to Archetypes with masks that have at least comopnentns of mask m
        /// </summary>
        /// <param name="m"></param>
        /// <returns></returns>
        std::vector<Archetype*> FindArchetypesWithMask(const mask& m) const;
        
        /// <summary>
        /// Returns vector of pointers to Archetypes with masks that have at least components T...
        /// </summary>
        /// <typeparam name="...T"></typeparam>
        /// <returns></returns>
        template<typename... T>
        std::vector<Archetype*> FindArchetypesWithMask() const;
        
        /// <summary>
        /// Registers System for Update cycle
        /// </summary>
        /// <param name="fn"></param>
        void RegisterUpdateSystem(SystemFn fn);

        /// <summary>
        /// Registers System for Input Cycle
        /// </summary>
        /// <param name="fn"></param>
        void RegisterInputSystem(SystemFn fn);

        /// <summary>
        /// Registers System for Render Cycle
        /// </summary>
        /// <param name="fn"></param>
        void RegisterRenderSystem(SystemFn fn);
        
        /// <summary>
        /// Updates Systems in Update Cycle
        /// </summary>
        /// <param name="context"></param>
        /// <param name="dt"></param>
        void UpdateSystems(void* context, float dt);

        /// <summary>
        /// Updates Systems in Input Cycle
        /// </summary>
        /// <param name="context"></param>
        /// <param name="dt"></param>
        void UpdateInputSystems(void* context, float dt);

        /// <summary>
        /// Updates Systems in Render Cycle
        /// </summary>
        /// <param name="context"></param>
        /// <param name="dt"></param>
        void UpdateRenderSystems(void* context, float dt);
        
        /// <summary>
        /// Get Id of Component T
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        template<typename T>
        ComponentID GetComponentId();

        /// <summary>
        /// Outputs all archetypes and masks
        /// </summary>
        void Test()
        {
            std::cout << "testing\n";
            for (const auto& [archMask, archetype] : archetypes)
            {
                std::cout << "Archetype Mask: " << archMask << ", Entity Count: " << archetype->GetEntityCount() << '\n';
            }
        }
        
    private:
        EntityID NextId = 1;
        ComponentFactory factory;
        std::unordered_map<mask, std::unique_ptr<Archetype>> archetypes;
        std::unordered_map<EntityID, Archetype*> entityToArchetype;
        std::vector<SystemFn> updateSystems;
        std::vector<SystemFn> inputSystems;
        std::vector<SystemFn> renderSystems;
    };
    
    /*template<typename T>
    inline void ecs::AddComponent(EntityID entity, T&& component)
    {
        using CleanT = std::decay_t<T>;
        ComponentID id = factory.GetId<CleanT>();
        mask newMask;

        auto it = entityToArchetype.find(entity);

        std::unordered_map<ComponentID, void*> componentData;
        componentData[id] = new CleanT(std::forward<T>(component));
        newMask.set(id);

        if (it == entityToArchetype.end()) {
            // New entity
            auto newArch = std::make_unique<Archetype>(newMask);
            newArch->AddEntity(entity, componentData);
            entityToArchetype[entity] = newArch.get();
            archetypes[newMask] = std::move(newArch);
            return;
        }

        // Existing entity — add to new archetype
        Archetype* oldArch = it->second;

        // SAFELY get component data before modifying maps
        auto existingData = oldArch->GetAllComponentsForEntity(entity);
        existingData[id] = new CleanT(std::forward<T>(component));

        mask updatedMask = oldArch->GetSignature();
        updatedMask.set(id);

        auto found = archetypes.find(updatedMask);
        Archetype* newArch = nullptr;

        if (found == archetypes.end()) {
            auto temp = std::make_unique<Archetype>(updatedMask);
            newArch = temp.get();
            archetypes[updatedMask] = std::move(temp);
        }
        else {
            newArch = found->second.get();
        }

        newArch->AddEntity(entity, existingData);
        oldArch->RemoveEntity(entity);
        entityToArchetype[entity] = newArch;
    }*/


    template<typename T>
    inline void ecs::AddComponent(EntityID entity)
    {
        ComponentID id = factory.GetId<T>();
        mask newMask;

        void* newComponentData = new T();
        std::unordered_map<ComponentID, void*> componentData;
        componentData[id] = newComponentData;
        newMask.set(id);

        auto it = entityToArchetype.find(entity);

        if (it == entityToArchetype.end())
        {
            auto newArch = std::make_unique<Archetype>(newMask);
            newArch->AddEntity(entity, componentData);
            entityToArchetype[entity] = newArch.get();
            archetypes[newMask] = std::move(newArch);
            return;
        }

        Archetype* oldArch = it->second;
        mask oldMask = oldArch->GetSignature();

        if (oldMask.test(id)) {
            std::cerr << "Component already exists for entity: " << entity << std::endl;
            delete static_cast<T*>(newComponentData);
            return;
        }

        std::unordered_map<ComponentID, void*> existingData = oldArch->GetAllComponentsForEntity(entity);
        existingData[id] = newComponentData;

        mask updatedMask = oldMask;
        updatedMask.set(id);

        Archetype* newArch = nullptr;
        auto found = archetypes.find(updatedMask);
        if (found == archetypes.end()) {
            auto temp = std::make_unique<Archetype>(updatedMask);
            newArch = temp.get();
            archetypes[updatedMask] = std::move(temp);
        }
        else {
            newArch = found->second.get();
        }

        newArch->AddEntity(entity, existingData);
        oldArch->RemoveEntity(entity);
        entityToArchetype[entity] = newArch;
    }

    template<typename T>
    inline void ecs::AddComponentData(EntityID entity, T component)
    {
        ComponentID id = factory.GetId<T>();
        mask newMask;

        void* newComponentData = new T(component);
        std::unordered_map<ComponentID, void*> componentData;
        componentData[id] = newComponentData;
        newMask.set(id);

        auto it = entityToArchetype.find(entity);

        if (it == entityToArchetype.end())
        {
            auto newArch = std::make_unique<Archetype>(newMask);
            newArch->AddEntity(entity, componentData);
            entityToArchetype[entity] = newArch.get();
            archetypes[newMask] = std::move(newArch);
            return;
        }

        Archetype* oldArch = it->second;
        mask oldMask = oldArch->GetSignature();

        if (oldMask.test(id)) {
            std::cerr << "Component already exists for entity: " << entity << std::endl;
            delete static_cast<T*>(newComponentData);
            return;
        }

        std::unordered_map<ComponentID, void*> existingData = oldArch->GetAllComponentsForEntity(entity);
        existingData[id] = newComponentData;

        mask updatedMask = oldMask;
        updatedMask.set(id);

        Archetype* newArch = nullptr;
        auto found = archetypes.find(updatedMask);
        if (found == archetypes.end()) {
            auto temp = std::make_unique<Archetype>(updatedMask);
            newArch = temp.get();
            archetypes[updatedMask] = std::move(temp);
        }
        else {
            newArch = found->second.get();
        }

        newArch->AddEntity(entity, existingData);
        oldArch->RemoveEntity(entity);
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