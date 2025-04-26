#include "renderer.hpp"
#include <glm/gtc/type_ptr.hpp>

Renderer::DrawData Renderer::orderData(Mesh& mesh)
{
	DrawData ret_data;
	ret_data.vertices = (float*)mesh.vertices;
	ret_data.indices = (GLuint*)malloc(sizeof(GLuint) * 3 * mesh.faceCount);
	if (ret_data.indices)
	{
		for (int i = 0; i < mesh.faceCount; i++)
		{
			ret_data.indices[i * 3] = mesh.faces[i].index[0];
			ret_data.indices[i * 3 + 1] = mesh.faces[i].index[1];
			ret_data.indices[i * 3 + 2] = mesh.faces[i].index[2];
		}
	}
	else
	{
		printf("Failed to allocate for index ordering!\n");
	}
	return ret_data;
}

void Renderer::cleanupData(DrawData data)
{
	data.vertices = NULL;
	if (data.indices) { free(data.indices); }
}


// pirms to sito atkomente un izdzes manu implementacije
// 1) tev tad vajag ari mesh izdzest mainigos dazus un UploadToGpu funkciju un ari Transform funckciju GetMatrix
// 2) es palasiju ka sis kods ir diezgan neefektivs un katru reizi sito setapot ir diezgan loti leni

/*int Renderer::DrawMesh(Mesh& mesh, Shader shader)
{
	int ret_val = 0;

	// Ready data for drawing
	float* vertices = NULL;
	unsigned int* indices = NULL;
	DrawData data = orderData(mesh);
	if (!data.vertices)
	{
		printf("No vertices to draw!\n");
		ret_val = 1; goto close;
	}
	vertices = data.vertices;
	if (!data.indices)
	{
		printf("No indices to draw!\n");
		ret_val = 1; goto close;
	}
	indices = data.indices;

	if (!shader.GetID())
	{
		printf("No shader to use!\n");
		ret_val = 1; goto close;
	}
	shader.Use();

	// Draw the mesh
	GLuint VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh.vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.faceCount * sizeof(GLuint) * 3, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); // Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float))); // Colour
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawElements(GL_TRIANGLES, mesh.faceCount * 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);

close:
	cleanupData(data);
	return ret_val;
}*/

int Renderer::DrawMesh(Mesh& mesh, Shader shader, glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	if (!mesh.uploaded)
	{
		mesh.UploadToGPU();
	}

	if (!shader.GetID())
	{
		printf("No shader to use!\n");
		return 1;
	}

	shader.Use();


	unsigned int modelLoc = glGetUniformLocation(shader.GetID(), "model");
	unsigned int viewLoc = glGetUniformLocation(shader.GetID(), "view");
	unsigned int projLoc = glGetUniformLocation(shader.GetID(), "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mesh.transform.GetMatrix()));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


	glBindVertexArray(mesh.VAO);
	glDrawElements(GL_TRIANGLES, mesh.faceCount * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	return 0;
}




void Renderer::Clear(glm::vec4 colour)
{
	glClearColor(colour.r, colour.g, colour.b, colour.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
