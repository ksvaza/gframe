#pragma once
#include "Types.hpp"
#include "ComponentFactory.hpp"
#include <unordered_map>

namespace ECS {

    class Archetype
    {
    public:
        Archetype(const mask& signature);

        void AddEntity(EntityID entity, const std::unordered_map<ComponentID, void*>& componentData);
        
        void RemoveEntity(EntityID entity);

        void* GetComponent(ComponentID id, size_t index);

        template<typename T>
        T* GetComponent(size_t index);

        const mask& GetSignature() const;

        bool Matches(const mask& query) const;

        size_t GetEntityCount() const;

        EntityID GetEntityAt(size_t index) const;
        
        void* GetComponentForEntity(EntityID entity, ComponentID id);

        std::unordered_map<ComponentID, void*> GetAllComponentsForEntity(EntityID entity);

        template<typename... T>
        bool hasComponents() const;
    private:
        mask signature;
        std::vector<EntityID> entityIDs;
        std::unordered_map<EntityID, size_t> entityToIndex;
        std::unordered_map<ComponentID, std::vector<std::byte>> componentArrays;
    };

    template<typename T>
    inline T* Archetype::GetComponent(size_t index)
    {
        ComponentID id = ComponentFactory::GetId<T>();
        if (!signature.test(id))
            return nullptr;

        size_t size = ComponentFactory::GetSize(id);
        auto& array = componentArrays[id];
        return reinterpret_cast<T*>(array.data() + index * size);
    }

    template<typename ...T>
    inline bool Archetype::hasComponents() const
    {
        return ((signature.test(ComponentFactory::GetId<T>())) && ...);
    }
}