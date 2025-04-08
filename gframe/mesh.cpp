#include "mesh.hpp"

Mesh::Mesh()
{
    vertices = NULL;
    vertexCount = 0;
    faces = NULL;
    faceCount = 0;
    textureReferences = NULL;
    textureRefCount = 0;
    colour = glm::vec4(1.0);
}

void Mesh::Create(int vertexCount, int faceCount)
{
    if (vertices) { free(vertices); }
    this->vertexCount = vertexCount;
    vertices = (Vertex*)malloc(sizeof(Vertex) * vertexCount);
    if (!vertices) { Delete(); }

    if (faces) { free(faces); }
    this->faceCount = faceCount;
    faces = (Face*)malloc(sizeof(Face) * faceCount);
    if (!faces) { Delete(); }
}

void Mesh::Delete()
{
    if (vertices) { free(vertices); }
    if (faces) { free(faces); }
    if (textureReferences) { free(textureReferences); }
}

std::string Mesh::sPrint()
{
    std::string retval = 
        "Transform:\n"
        "{\n\tPosition: (" + std::to_string(transform.position.x) + ", " + std::to_string(transform.position.y) + ", " + std::to_string(transform.position.z) + ")\n"
        "\tRotation: (" + std::to_string(transform.rotation.x) + ", " + std::to_string(transform.rotation.y) + ", " + std::to_string(transform.rotation.z) + ")\n"
        "\tRotation: (" + std::to_string(transform.scale.x) + ", " + std::to_string(transform.scale.y) + ", " + std::to_string(transform.scale.z) + ")\n}\n"
        "Mesh vertices: " + std::to_string(vertexCount) + "\n";

    for (int i = 0; i < vertexCount; i++)
    {
        retval.append(
            "Vertex " + std::to_string(i) + ": {\n\tPosition: (" + std::to_string(vertices[i].x) + ", " + std::to_string(vertices[i].y) + ", " + std::to_string(vertices[i].z) + ")\n"
            "\tColour: (" + std::to_string(vertices[i].r) + ", " + std::to_string(vertices[i].g) + ", " + std::to_string(vertices[i].b) + ", " + std::to_string(vertices[i].a) + ")\n"
            "\tTexCoords: (" + std::to_string(vertices[i].u) + ", " + std::to_string(vertices[i].v) + ")\n}\n"
        );
    }

    retval.append("Mesh faces: " + std::to_string(faceCount) + "\n");

    for (int i = 0; i < faceCount; i++)
    {
        retval.append(
            "Face " + std::to_string(i) + ": {\n\tIndices: (" + std::to_string(faces[i].index[0]) + ", " + std::to_string(faces[i].index[1]) + ", " + std::to_string(faces[i].index[2]) + ")\n"
            "\tNormal: (" + std::to_string(faces[i].normal.x) + ", " + std::to_string(faces[i].normal.y) + ", " + std::to_string(faces[i].normal.z) + ")\n"
            "\tTexID: (" + std::to_string(faces[i].textureID) + ")\n}\n"
        );
    }

    return retval;
}

void Mesh::Print()
{
	printf("%s\n", sPrint().c_str());
	return;
}

std::ostream& operator<<(std::ostream& os, const Mesh& mesh)
{
	os << ((Mesh)mesh).sPrint();
	return os;
}
