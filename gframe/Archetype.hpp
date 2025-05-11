#pragma once
#include <unordered_map>
#include <typeindex>
#include <any>
#include "Position.hpp"
#include <memory>
#include "BaseComponent.hpp"
#include <bitset>

namespace EECS
{
	using EntityId = uint32_t;
	using ComponentMask = std::bitset<64>;//max 64 components !for now!
	template<typename ...T>
	class Archetype
	{
	public:
		ComponentMask mask;
		Archetype()
		{
			(createVectorForType<T>(), ...);
		}
		~Archetype() {};
		template<typename ...Component>
		void AddComponent()
		{
			(createVectorForType<Component>(), ...);
		}
		void AddEntity(EntityId entity)
		{
			Entities.push_back(entity);
			for (auto& x : map)
			{
				x.second->resize(Entities.size());
			}
		}
		void RemoveEntity(EntityId entity)
		{
			auto it = std::find(Entities.begin(), Entities.end(), entity);
			if (it != Entities.end())
			{
				size_t index = std::distance(Entities.begin(), it);
				Entities.erase(it);

				for (auto& typePair : map)
				{
					typePair.second->erase(typePair.second->begin() + index);
				}
			}
			if (Entities.empty())
			{
				this->~Archetype();
			}
		}
	private:
		std::vector<EntityId> Entities;
		std::unordered_map<std::type_index, std::unique_ptr<std::vector<std::unique_ptr<BaseComponent>>>> map;
		template <typename Component>
		void createVectorForType()
		{
			std::type_index typeIndex = std::type_index(typeid(Component));
			mask.set((int)ComponentType::getId(typeIndex));
			if (map.find(typeIndex) == map.end())
			{
				map[typeIndex] = std::make_unique<std::vector<std::unique_ptr<BaseComponent>>>();
			}
		}
	};


	template<typename ...T>
	ComponentMask CreateMask()
	{
		ComponentMask mask;
		(mask.set(ComponentType::getId(std::type_index(typeid(T)))), ...);
		return mask;
	}
}
