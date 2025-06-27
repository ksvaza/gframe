#pragma once
#include "Types.hpp"
#include "ComponentFactory.hpp"
#include <unordered_map>

namespace ECS {

    class Archetype
    {
    public:
        /// <summary>
        /// Creates new Archetype with mask signature.
        /// </summary>
        /// <param name="signature"></param>
        Archetype(const mask& signature);

        /// <summary>
        /// Adds entity to Archetype with given component data.
        /// </summary>
        /// <param name="entity"></param>
        /// <param name="componentData"></param>
        void AddEntity(EntityID entity, const std::unordered_map<ComponentID, void*>& componentData);
        
        /// <summary>
        /// Removes entityID from Archetype.
        /// </summary>
        /// <param name="entity"></param>
        void RemoveEntity(EntityID entity);

        /// <summary>
        /// Returns void pointer to component of ComponentID id at index in Archetype.
        /// </summary>
        /// <param name="id"></param>
        /// <param name="index"></param>
        /// <returns></returns>
        void* GetComponent(ComponentID id, size_t index);

        /// <summary>
        /// Returns pointer to component of type T at index in Archetype.
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="index"></param>
        /// <returns></returns>
        template<typename T>
        T* GetComponent(size_t index);

        /// <summary>
        /// Returns mask signature of Archetype, which is a bitset of ComponentIDs that this Archetype contains.
        /// </summary>
        /// <returns></returns>
        const mask& GetSignature() const;

        /// <summary>
        /// Returns true if Archetype matches the query mask, meaning it contains all components in the query.
        /// </summary>
        /// <param name="query"></param>
        /// <returns></returns>
        bool Matches(const mask& query) const;

        /// <summary>
        /// Returns the number of entities in this Archetype.
        /// </summary>
        /// <returns></returns>
        size_t GetEntityCount() const;

        /// <summary>
        /// Returns EntityID at index in Archetype.
        /// </summary>
        /// <param name="index"></param>
        /// <returns></returns>
        EntityID GetEntityAt(size_t index) const;
        
        /// <summary>
        /// Returns pointer to component of ComponentID id for the given entity.
        /// </summary>
        /// <param name="entity"></param>
        /// <param name="id"></param>
        /// <returns></returns>
        void* GetComponentForEntity(EntityID entity, ComponentID id);

        /// <summary>
        /// Returns unordered_map of all components for the given entity.
        /// </summary>
        /// <param name="entity"></param>
        /// <returns></returns>
        std::unordered_map<ComponentID, void*> GetAllComponentsForEntity(EntityID entity);

        /// <summary>
        /// Returns true if Archetype has all components of types T.
        /// </summary>
        /// <typeparam name="...T"></typeparam>
        /// <returns></returns>
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