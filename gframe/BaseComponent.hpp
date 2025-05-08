#pragma once
#include <unordered_map>
#include <typeindex>

static class ComponentType
{
	static std::unordered_map<std::type_index, std::uint64_t> ComponentIdMap;
	static std::uint64_t ComponentIdCounter;
public:
	static std::uint64_t getId(std::type_index type)
	{
		auto it = ComponentIdMap.find(type);
		
		if (it != ComponentIdMap.end())
		{
			return ComponentIdMap[type];
		}
		ComponentIdMap[type] = ComponentIdCounter;
		ComponentIdCounter++;
		return ComponentIdMap[type];
	}
	static void reset()
	{
		ComponentIdMap.clear();
		ComponentIdCounter = 0;
	}
};




class BaseComponent
{
public:
	virtual ~BaseComponent();
};