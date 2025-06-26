#pragma once
#include <glm/glm.hpp>
#include "Types.hpp"

struct AABB3d
{
	glm::vec3 MinPos;
	glm::vec3 MaxPos;
};

class Collider
{
	AABB3d bounds;
	EntityID CollidesWith;
};