#include "transform.hpp"

Transform::Transform()
{
	position = glm::vec3(0.0);
	rotation = glm::vec3(0.0);
	scale = glm::vec3(0.0);
}

Transform::Transform(glm::vec3 position)
{
	this->position = position;
	rotation = glm::vec3(0.0);
	scale = glm::vec3(0.0);
}

Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}

bool Transform::Equal(Transform& transform)
{
	if (position != transform.position) { return false; }
	if (rotation != transform.rotation) { return false; }
	if (scale != transform.scale) { return false; }
	return true;
}
