#pragma once
#include <stdlib.h>
#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "mesh.hpp"
#include "shader.hpp"

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
	int DrawMesh(Mesh& mesh, Shader shader);
	void Clear(glm::vec4 colour);
};
