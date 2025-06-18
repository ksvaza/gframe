#pragma once
#include <stdlib.h>
#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "mesh.hpp"
#include "shader.hpp"

class Renderer
{
private:
	struct DrawData
	{
		float* vertices;
		GLuint* indices;
	};

	struct dVertex
	{
		
	};
	struct dFace
	{
		GLuint i, j, k;
	};

	class MeshSnapshot
	{
	public:
		int vertexCount = 0;
		int faceCount = 0;
		Transform transform;
		glm::vec4 colour = glm::vec4(1.0f);

		void Capture(Mesh& mesh);
		bool Equals(Mesh& mesh);

		MeshSnapshot();
		~MeshSnapshot() {}
	};

	static DrawData orderData(Mesh& mesh);
	static void cleanupData(DrawData data);
public:
	class Batch
	{
	public:
		Mesh** meshes = NULL;
		MeshSnapshot* meshSnapshots = NULL;
		int meshCount = 0;
		int totalVertexCount = 0;
		int totalFaceCount = 0;
		GLuint VAO, VBO, EBO;

		int AddMesh(Mesh& mesh);
		int OrderAndMapData();
		void FreeData();
		int UpdateData();
		int Draw(Shader shader);
		Batch();
		~Batch();
	};


	//static int DrawMesh(Mesh& mesh, Shader shader);
	//static void Clear(glm::vec4 colour);
	int DrawMesh(Mesh& mesh, Shader shader, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
	void Clear(glm::vec4 colour);
};
