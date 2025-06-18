#include "transform.hpp"
#include <glm/gtc/matrix_transform.hpp>

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


glm::mat4 Transform::GetMatrix() const
{
    glm::mat4 matrix = glm::mat4(1.0f);

    matrix = glm::translate(matrix, position);

    matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));

    matrix = glm::scale(matrix, scale);

    return matrix;
}
bool Transform::Equal(Transform& transform)
{
	if (position != transform.position) { return false; }
	if (rotation != transform.rotation) { return false; }
	if (scale != transform.scale) { return false; }
	return true;
}
