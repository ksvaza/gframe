#pragma once
#include <set>
#include "Archetype.hpp"

class ECS
{
public:
	EntityId CreateEntity()
	{
		return EntityId;
		EntityId++;
	}
	template<typename T>
	void AttachComponent(EntityId entity)
	{
		if (entityMasks[entity][ComponentType::getId(std::type_index(typeid(T)))])
		{
			return;
		}
		ComponentMask mask = CreateMask<T>;
		Archetype* arch = FindExactMatch(entityMasks[entity]);
		arch->RemoveEntity(entity);
		entityMasks[entity].set(ComponentType::getId(std::type_index(typeid(T))));
		AddArchetype///da blin
	}
private:
	std::vector<Archetype<>> FindPartialMatch(ComponentMask mask)
	{
		std::vector<Archetype<>> vec;
		for (const auto& x : ArchetypePool)
		{
			if ((x.second.mask & mask) == mask)
			{
				vec.push_back(x.second);
			}
		}
		return vec;
	}
	Archetype<>* FindExactMatch(ComponentMask mask)
	{
		auto it = ArchetypePool.find(mask);
		if (it != ArchetypePool.end()) {
			return &it->second;
		}
		return nullptr;;
	}
	template<typename ...T>
	void AddArchetype()
	{
		ComponentMask mask = CreateMask<...T>();
		auto it = ArchetypePool.find(mask);
		if (it != ArchetypePool.end())
		{
			ArchetypePool[mask] = Archetype<...T>();
		}
	}
	std::unordered_map<ComponentMask, Archetype<>> ArchetypePool;
	EntityId EntityId;
	std::unordered_map<EntityId, ComponentMask> entityMasks;
};