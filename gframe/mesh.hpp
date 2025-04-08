#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
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
	std::string_view texName;
	// Buutu veel jaauzraksta AssetManager, lai sheit vareetu pabeigt liidz galam.
};

class Mesh
{
public:
	Vertex* vertices = NULL;
	int vertexCount = 0;

	Face* faces = NULL;
	int faceCount = 0;

	TextureRef* textureReferences = NULL;
	int textureRefCount = 0;

	Transform transform;
	glm::vec4 colour = glm::vec4(1.0);

	int Create(int vertexCount, int faceCount);
	int CreateV(int vertexCount);
	int CreateF(int faceCount);
	int Recreate(int vertexCount, int faceCount);
	int RecreateV(int vertexCount);
	int RecreateF(int faceCount);
	void Delete();

	std::string sPrint();
	void Print();

	bool Equals(Mesh& mesh);
	bool IsEmpty();

	Mesh();
	~Mesh() {}

	class Construct
	{
	public:
		static void Rectangle(Mesh& mesh, glm::vec2 position, glm::vec2 size);
		static void Circle(Mesh& mesh, glm::vec2 position, float radius, int segments);
		// Low level mesh vertex generation
	};
	class Bake
	{
	public:
		static int Triangles(Mesh& mesh);
		static int Lines(Mesh& mesh);
		// Options for building the faces of the mesh
	};
	class Make
	{
	public:
		// Higher level operations on meshes
	};
	class Modify
	{
	public:
		static void Colour(Mesh& mesh, glm::vec4 colour);

		static int Append(Mesh& mesh, Mesh& add);
	};
};
std::ostream& operator<<(std::ostream& os, const Mesh& mesh);
