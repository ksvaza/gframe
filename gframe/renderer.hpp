#pragma once
#include <stdlib.h>
#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "mesh.hpp"
#include "shader.hpp"
#include "MeshComponent.hpp"
#include "Camera.hpp"
#include "TransformComponent.hpp"

class Renderer
{
	struct DrawData
	{
		float* vertices;
		GLuint* indices;
	};
private:
	DrawData orderData(Mesh& mesh);
	void cleanupData(DrawData data);
public:
	int DrawMesh(Mesh& mesh, Shader shader, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
	void Clear(glm::vec4 colour);

	int EcsDrawMesh(TransformComponent transform, MeshComponent& mesh, Shader shader, Camera& camera);//es pievienosu metoditi oki?
};
