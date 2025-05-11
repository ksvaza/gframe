#pragma once
#include <unordered_map>
#include <vector>
#include <typeindex>
#include "Archetype.hpp"

namespace EECS
{
	using EntityId = uint32_t;

	class ECS
	{
	public:
		ECS() : nextEntityId(1) {}

		EntityId CreateEntity()
		{
			return nextEntityId++;
		}

		void AttachComponent(EntityId entity)
		{
			auto it = entityMasks.find(entity);
			if (it != entityMasks.end())
			{
				if (it->second[ComponentType::getId(std::type_index(typeid(Position)))])
				{
					return; // Component already attached
				}

				// Find current archetype
				Archetype<Position>* current = FindExactMatch(it->second);
				if (current)
					current->RemoveEntity(entity);

				// Update mask with Position component
				it->second.set(ComponentType::getId(std::type_index(typeid(Position))));

				// Find or create new archetype
				Archetype<Position>* newArch = FindExactMatch(it->second);
				if (!newArch)
				{
					AddArchetype<Position>();
					newArch = FindExactMatch(it->second);
				}

				if (newArch)
					newArch->AddEntity(entity);
			}
		}

	private:
		std::vector<Archetype<Position>*> FindPartialMatch(const ComponentMask& mask)
		{
			std::vector<Archetype<Position>*> vec;
			for (auto& x : ArchetypePool)
			{
				if ((x.first & mask) == mask)
				{
					vec.push_back(&x.second);
				}
			}
			return vec;
		}

		Archetype<Position>* FindExactMatch(const ComponentMask& mask)
		{
			auto it = ArchetypePool.find(mask);
			if (it != ArchetypePool.end()) {
				return &it->second;
			}
			return nullptr;
		}

		template<typename... T>
		void AddArchetype()
		{
			ComponentMask mask = CreateMask<T...>();
			if (ArchetypePool.find(mask) == ArchetypePool.end())
			{
				ArchetypePool.emplace(mask, Archetype<T...>());
			}
		}

		std::unordered_map<ComponentMask, Archetype<Position>> ArchetypePool;
		EntityId nextEntityId;
		std::unordered_map<EntityId, ComponentMask> entityMasks;
	};

	static void test()
	{
		ECS ecs;
		EntityId entity = ecs.CreateEntity();
		ecs.AttachComponent(entity);
	}
}
