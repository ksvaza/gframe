#pragma once
#include <unordered_map>
#include <typeindex>
#include <any>
#include "Position.hpp"
#include "Velocity.hpp"
#include <memory>
#include "BaseComponent.hpp"
#include <bitset>
#include <cassert>

namespace ECS
{
	using Entity = uint32_t;
	using ComponentMask = std::bitset<64>;//max 64 components !for now!

	constexpr size_t MAX_COMPONENTS = 64;
	using ComponentMask = std::bitset<MAX_COMPONENTS>;

	class ComponentTypeRegistry 
	{
		static inline size_t typeCounter = 0;
		static inline std::unordered_map<std::type_index, size_t> typeToId;

	public:
		template<typename T>
		static size_t getId()
		{
			std::type_index type(typeid(T));
			auto it = typeToId.find(type);
			if (it != typeToId.end())
				return it->second;

			assert(typeCounter < MAX_COMPONENTS && "Too many components!");
			size_t id = typeCounter++;
			typeToId[type] = id;
			return id;
		}
	};


	class BaseComponentArray {
	public:
		virtual ~BaseComponentArray() = default;
		virtual void remove(size_t index) = 0;
		virtual void move(size_t from, size_t to) = 0;
		virtual void* getRaw(size_t index) = 0;
		virtual void resize(size_t newSize) = 0;
		virtual void reserve(size_t capacity) = 0;
	};

	template<typename T>
	class ComponentArray : public BaseComponentArray {
	public:
		std::vector<T> data;

		void remove(size_t index) override
		{
			data[index] = std::move(data.back());
			data.pop_back();
		}
		void move(size_t from, size_t to) override 
		{
			data[to] = std::move(data[from]);
		}
		void* getRaw(size_t index) override
		{
			return &data[index];
		}
		void resize(size_t newSize) override 
		{
			data.resize(newSize);
		}
		void reserve(size_t capacity) override 
		{
			data.reserve(capacity);
		}
		T& get(size_t index) 
		{
			return data[index];
		}
		void add(const T& component)
		{
			data.push_back(component);
		}
	};

	class Archetype
	{
	private:
		ComponentMask mask;
		std::unordered_map<std::type_index, std::unique_ptr<BaseComponentArray>> componentArrays;
		std::vector<Entity> entities;
		size_t entityCount = 0;
	public:
		
		template<typename ...T>
		void Init()
		{
			(registerComponent<T>(), ...);
		}
		template<typename T>
		void registerComponent()
		{
			const std::type_index index(typeid(T));
			if (componentArrays.find(index) == componentArrays.end()) 
			{
				componentArrays[index] = std::make_unique<ComponentArray<T>>();
				mask.set(ComponentTypeRegistry::getId<T>());
			}
		}
		ComponentMask getComponentMask() const
		{
			return mask;
		}
		template<typename... Components>
		void addEntity(Entity entity, Components&&... components)
		{
			(addComponent(std::forward<Components>(components)), ...);
			entities.push_back(entity);
			entityCount++;
		}
		template<typename T>
		void addComponent(const T& component) 
		{
			auto* array = getComponentArray<T>();
			array->add(component);
		}
		void removeEntity(size_t index) 
		{
			assert(index < entityCount);
			size_t last = entityCount - 1;

			for (auto& [type, array] : componentArrays) 
			{
				array->remove(index);
			}

			entities[index] = entities[last];
			entities.pop_back();
			entityCount--;
		}
		template<typename T>
		T& getComponent(size_t index) 
		{
			auto* array = getComponentArray<T>();
			return array->get(index);
		}
		template<typename T>
		ComponentArray<T>* getComponentArray() 
		{
			const std::type_index index(typeid(T));
			auto it = componentArrays.find(index);
			assert(it != componentArrays.end());
			return static_cast<ComponentArray<T>*>(it->second.get());
		}
		size_t size() const 
		{
			return entityCount;
		}
		const std::vector<Entity>& getEntities() const
		{
			return entities;
		}
	};

	//helperitis
	template<typename... Components>
	ComponentMask createMask()
	{
		ComponentMask mask;
		(mask.set(ComponentTypeRegistry::getId<Components>()), ...);
		return mask;
	}
}
