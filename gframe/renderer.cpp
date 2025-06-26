#include "renderer.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <vector>

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

/*int Renderer::EcsDrawMesh(TransformComponent transform, MeshComponent& mesh, Shader shader, Camera& camera)
{
	if (!mesh.isUploaded)
	{
		glGenVertexArrays(1, &mesh.VAO);
		glGenBuffers(1, &mesh.VBO);
		glGenBuffers(1, &mesh.EBO);

		glBindVertexArray(mesh.VAO);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh.vertexCount * sizeof(Vertex), mesh.vertices, GL_STATIC_DRAW);

		std::vector<unsigned int> indices;
		for (int i = 0; i < mesh.faceCount; i++)
		{
			indices.push_back(mesh.faces[i].index[0]);
			indices.push_back(mesh.faces[i].index[1]);
			indices.push_back(mesh.faces[i].index[2]);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);

		mesh.isUploaded = true;
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

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform.GetMatrix()));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));


	glBindVertexArray(mesh.VAO);
	glDrawElements(GL_TRIANGLES, mesh.faceCount * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	return 0;
}*/
