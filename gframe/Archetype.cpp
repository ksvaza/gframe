#include "Archetype.hpp"
#include <iostream>

ECS::Archetype::Archetype(const mask& signature)
{
    this->signature = signature;
    for (ComponentID id = 0; id < MAX_COMPONENTS; id++) 
    {
        if (signature.test(id)) 
        {
            componentArrays[id] = std::vector<std::byte>();
        }
    }
}

void ECS::Archetype::AddEntity(EntityID entity, const std::unordered_map<ComponentID, void*>& componentData)
{
    size_t index = entityIDs.size();
    entityIDs.push_back(entity);
    entityToIndex[entity] = index;

    for (ComponentID id = 0; id < MAX_COMPONENTS; id++) 
    {
        if (!signature.test(id)) continue;

        if (componentData.find(id) == componentData.end()) {
            std::cerr << "ERROR: componentData is missing component ID " << id << std::endl;
            continue; // Or crash with a helpful error
        }


        auto& array = componentArrays[id];
        size_t size = ComponentFactory::GetSize(id);


        const std::byte* src = reinterpret_cast<const std::byte*>(componentData.at(id));

        array.insert(array.end(), src, src + size);
    }
}

void ECS::Archetype::RemoveEntity(EntityID entity)
{
    auto it = entityToIndex.find(entity);
    if (it == entityToIndex.end())
        return;

    size_t index = it->second;
    size_t lastIndex = entityIDs.size() - 1;
    EntityID lastEntity = entityIDs[lastIndex];

    if (index != lastIndex) 
    {
        entityIDs[index] = lastEntity;
        entityToIndex[lastEntity] = index;

        for (ComponentID id = 0; id < MAX_COMPONENTS;id++) 
        {
            if (!signature.test(id)) continue;

            auto& array = componentArrays[id];
            size_t size = ComponentFactory::GetSize(id);

            std::byte* data = array.data();
            std::memcpy(data + index * size, data + lastIndex * size, size);
        }
    }

    entityIDs.pop_back();
    entityToIndex.erase(entity);
    for (ComponentID id = 0; id < MAX_COMPONENTS; id++)
    {
        if (!signature.test(id)) continue;

        auto& array = componentArrays[id];
        size_t size = ComponentFactory::GetSize(id);
        array.resize(array.size() - size);
    }
}

void* ECS::Archetype::GetComponent(ComponentID id, size_t index)
{
    if (!signature.test(id))
        return nullptr;

    size_t size = ComponentFactory::GetSize(id);
    auto& array = componentArrays[id];
    return array.data() + index * size;
}

const mask& ECS::Archetype::GetSignature() const
{
    return signature;
}

bool ECS::Archetype::Matches(const mask& query) const
{
    return (signature & query) == query;
}

size_t ECS::Archetype::GetEntityCount() const
{
    return entityIDs.size();
}

EntityID ECS::Archetype::GetEntityAt(size_t index) const
{
    return entityIDs[index];
}

void* ECS::Archetype::GetComponentForEntity(EntityID entity, ComponentID id)
{
    if (!signature.test(id)) return nullptr;
    auto it = entityToIndex.find(entity);
    if (it == entityToIndex.end()) return nullptr;

    size_t index = it->second;
    size_t size = ComponentFactory::GetSize(id);
    auto& array = componentArrays[id];
    return array.data() + index * size;
}

std::unordered_map<ComponentID, void*> ECS::Archetype::GetAllComponentsForEntity(EntityID entity)
{
    std::unordered_map<ComponentID, void*> result;
    auto it = entityToIndex.find(entity);
    
    if (it == entityToIndex.end()) 
        return result;
    
    size_t index = it->second;

    for (ComponentID id = 0; id < MAX_COMPONENTS;id++)
    {
        if (!signature.test(id)) continue;
        
        size_t size = ComponentFactory::GetSize(id);
        auto& array = componentArrays[id];
        std::byte* base = array.data();
        size_t offset = index * size;
        if (offset + size <= array.size()) {
            result[id] = static_cast<void*>(base + offset);
        }
        else {
            std::cerr << "ERROR: Invalid access in GetAllComponentsForEntity for component ID " << id << std::endl;
        }


        //result[id] = ptr;
    }

    return result;
}