#include "BaseComponent.hpp"

namespace EECS
{
	std::unordered_map<std::type_index, std::uint32_t> ComponentType::ComponentIdMap;
	std::uint32_t ComponentType::ComponentIdCounter = 0;
}
