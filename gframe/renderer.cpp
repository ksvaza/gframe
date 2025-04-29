#include "renderer.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <thread>

void Renderer::MeshSnapshot::Capture(Mesh& mesh)
{
	vertexCount = mesh.vertexCount;
	faceCount = mesh.faceCount;
	transform = mesh.transform;
	colour = mesh.colour;
}

bool Renderer::MeshSnapshot::Equals(Mesh& mesh)
{
	if (vertexCount != mesh.vertexCount) { return false; }
	if (faceCount != mesh.faceCount) { return false; }
	if (!transform.Equal(mesh.transform)) { return false; }
	if (colour != mesh.colour) { return false; }
	return true;
}

Renderer::MeshSnapshot::MeshSnapshot()
{
	vertexCount = 0;
	faceCount = 0;
	transform = Transform();
	colour = glm::vec4(1.0f);
}

Renderer::DrawData Renderer::orderData(Mesh& mesh)
{
	DrawData ret_data;
	if (mesh.faceCount < 1)
	{
		ret_data.vertices = NULL;
		ret_data.indices = NULL;
		return ret_data;
	}
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

int Renderer::Batch::AddMesh(Mesh& mesh)
{
	Mesh* meshPtr = &mesh;
	if (!meshPtr) { return 1; }
	if (meshCount == 0)
	{
		meshes = (Mesh**)malloc(sizeof(Mesh*));
		if (!meshes) { return 1; }
		meshSnapshots = (MeshSnapshot*)malloc(sizeof(MeshSnapshot*));
		if (!meshSnapshots) { return 1; }
	}
	else
	{
		void* temp = realloc(meshes, sizeof(Mesh*) * (meshCount + 1));
		if (!temp) { free(meshes); return 1; }
		meshes = (Mesh**)temp;
		temp = realloc(meshSnapshots, sizeof(MeshSnapshot*) * (meshCount + 1));
		if (!temp) { free(meshSnapshots); return 1; }
		meshSnapshots = (MeshSnapshot*)temp;
	}
	meshes[meshCount] = meshPtr;
	meshSnapshots[meshCount].Capture(mesh);
	mesh.changed = true;
	totalVertexCount += mesh.vertexCount;
	totalFaceCount += mesh.faceCount;
	meshCount++;

	return 0;
}

int Renderer::Batch::OrderAndMapData()
{
	if (VAO || VBO || EBO) { FreeData(); }
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, totalVertexCount * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);
	Vertex* vPtr = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (!vPtr) { return 1; }
	int ofs = 0;
	for (int i = 0; i < meshCount; i++)
	{
		if (!meshes[i]->vertices) { continue; }
		memcpy(vPtr + ofs, meshes[i]->vertices, meshes[i]->vertexCount/* * sizeof(Vertex)*/);
		ofs += meshes[i]->vertexCount;
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalFaceCount * sizeof(dFace), NULL, GL_DYNAMIC_DRAW);
	dFace* fPtr = (dFace*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (!fPtr) { return 1; }
	ofs = 0;
	int fOfs = 0;
	for (int i = 0; i < meshCount; i++)
	{
		for (int j = 0; j < meshes[i]->faceCount; j++)
		{
			Face face = meshes[i]->faces[j];
			fPtr[fOfs + j].i = face.index[0] + ofs;
			fPtr[fOfs + j].j = face.index[1] + ofs;
			fPtr[fOfs + j].k = face.index[2] + ofs;
		}
		ofs += meshes[i]->vertexCount;
		fOfs += meshes[i]->faceCount;
	}
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); // Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float))); // Colour
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return 0;
}

void Renderer::Batch::FreeData()
{
	if (VAO) { glDeleteVertexArrays(1, &VAO); }
	if (VBO) { glDeleteBuffers(1, &VBO); }
	if (EBO) { glDeleteBuffers(1, &EBO); }
}

int Renderer::Batch::UpdateData()
{
	printf("UpdateData\n");
	bool changes = false;
	for (int i = 0; i < meshCount; i++)
	{
		if (meshes[i]->changed)
		{
			changes = true;
			meshes[i]->changed = false;
			meshSnapshots[i].Capture(*meshes[i]);
			printf("Capture %d, ", i);
		}
	}
	printf("%cUpdateData1\n", changes ? '\n' : ' ');
	if (changes)
	{
		OrderAndMapData();
	}
	printf("UpdateData2\n");
	return 0;
}

int Renderer::Batch::Draw(Shader shader)
{
	UpdateData();
	printf("BDraw 1\n");
	shader.Use();

	printf("BDraw 2\n");
	glBindVertexArray(VAO);

	printf("BDraw 3\n");
	glDrawElements(GL_TRIANGLES, totalFaceCount * 3, GL_UNSIGNED_INT, 0);

	printf("BDraw 4\n");
	glBindVertexArray(0);

	printf("BDraw 5\n");

	return 0;
}

Renderer::Batch::Batch()
{
	meshes = NULL;
	meshCount = 0;
	totalVertexCount = 0;
	totalFaceCount = 0;
	VAO = 0;
	VBO = 0;
	EBO = 0;
}

Renderer::Batch::~Batch()
{
	free(meshes);
	free(meshSnapshots);
	meshCount = 0;
	//FreeData();
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

int Renderer::DrawMesh(Mesh& mesh, Shader shader)
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
	//unsigned int viewLoc = glGetUniformLocation(shader.GetID(), "view");
	//unsigned int projLoc = glGetUniformLocation(shader.GetID(), "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mesh.transform.GetMatrix()));
	//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


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
