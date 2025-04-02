#pragma once
#include "transform.hpp"
#include <glm/glm.hpp>
#include <string>

struct Vertex
{
	float x = 0.0, y = 0.0, z = 0.0;
	float r = 0.0, g = 0.0, b = 0.0, a = 0.0;
	float u = 0.0, v = 0.0;
};

struct Face
{
	int index[3] = { 0, 0, 0 };
	glm::vec3 normal = glm::vec3(0.0);
	int textureID = -1;
};

struct TextureRef
{
	std::string texName;
	// Buutu veel jaauzraksta AssetManager, lai sheit vareetu pabeigt liidz galam.
};

class Mesh
{
public:
	Vertex* vertices;
	int vertexCount = 0;

	Face* faces;
	int faceCount = 0;

	TextureRef* texureReferences;
	int textureRefCount;

	Transform transform;
	glm::vec4 colour;

	static Mesh Empty();

	void Create(int vertexCount, int faceCount);
	void Delete();

	void Print();
	std::string sPrint();

	bool Equals(Mesh* mesh);
	bool IsEmpty();
};
