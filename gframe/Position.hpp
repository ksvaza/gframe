#pragma once
#include "BaseComponent.hpp"
namespace ECS
{
	struct Position : BaseComponent
	{
		Position() = default;
		Position(float x, float y, float z) : x(x), y(y), z(z) {};
		float x, y, z;
	};
}
	