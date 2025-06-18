#pragma once
#include <glm/vec3.hpp>
#include <glm/geometric.hpp> 

class Orientation
{
public:
	Orientation(float yaw = 0.0f, float pitch = 0.0f)
		: yaw(yaw), pitch(pitch) {}
	float yaw;
	float pitch;
};