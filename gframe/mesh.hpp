#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "transform.hpp"
#include <glm/glm.hpp>
#include <string>
#include "UsefulStructs.hpp"

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

	void Create(int vertexCount, int faceCount);
	void Delete();

	std::string sPrint();
	void Print();

	bool Equals(Mesh& mesh);
	bool IsEmpty();

	Mesh();
	~Mesh() {}
};
std::ostream& operator<<(std::ostream& os, const Mesh& mesh);
