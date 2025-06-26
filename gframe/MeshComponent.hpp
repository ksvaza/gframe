#pragma once
#include <vector>
#include <iostream>

class MeshComponent
{
public:
	MeshComponent() = default;
	~MeshComponent() = default;
	MeshComponent(const MeshComponent& other) = default;

    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    std::vector<TextureRef> textureReferences;

    glm::vec4 colour = glm::vec4(1.0f);
    bool isUploaded = false;
};

