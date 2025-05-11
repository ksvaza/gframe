#pragma once
#include "BaseComponent.hpp"


namespace ECS
{
	struct Velocity : BaseComponent
	{
		Velocity() = default;
		Velocity(float x, float y, float z) : x(x), y(y), z(z) {};
		float x, y, z;
	};
}

