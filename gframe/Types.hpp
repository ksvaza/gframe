#pragma once
#include <cstdint>
#include <bitset>
#include <stdexcept>

using EntityID = std::uint64_t;
using ComponentID = std::uint32_t;
static constexpr size_t MAX_COMPONENTS = 64;
using mask = std::bitset<MAX_COMPONENTS>;