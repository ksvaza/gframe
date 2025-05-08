#include "BaseComponent.hpp"

std::unordered_map<std::type_index, std::uint64_t> ComponentType::ComponentIdMap;
std::uint64_t ComponentType::ComponentIdCounter = 0;