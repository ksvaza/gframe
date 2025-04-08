#include "mesh.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

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

int Mesh::Create(int vertexCount, int faceCount)
{
    if (vertexCount <= 0)
    {
        return 0;
    }
    if (vertices) { free(vertices); }
    this->vertexCount = vertexCount;
    vertices = (Vertex*)malloc(sizeof(Vertex) * vertexCount);
    if (!vertices)
    {
        printf("Failed to allocate vertices!\n");
        Delete();
        return 1;
    }

    if (faceCount <= 0)
    {
        return 0;
    }
    if (faces) { free(faces); }
    this->faceCount = faceCount;
    faces = (Face*)malloc(sizeof(Face) * faceCount);
    if (!faces)
    {
        printf("Failed to allocate faces!\n");
        Delete();
        return 1;
    }

    return 0;
}

int Mesh::CreateV(int vertexCount)
{
    if (vertexCount <= 0)
    {
        return 0;
    }
    if (vertices) { free(vertices); }
    this->vertexCount = vertexCount;
    vertices = (Vertex*)malloc(sizeof(Vertex) * vertexCount);
    if (!vertices)
    {
        printf("Failed to allocate vertices!\n");
        Delete();
        return 1;
    }

    return 0;
}

int Mesh::CreateF(int faceCount)
{
    if (faceCount <= 0)
    {
        return 0;
    }
    if (faces) { free(faces); }
    this->faceCount = faceCount;
    faces = (Face*)malloc(sizeof(Face) * faceCount);
    if (!faces)
    {
        printf("Failed to allocate faces!\n");
        Delete();
        return 1;
    }

    return 0;
}

int Mesh::Recreate(int vertexCount, int faceCount)
{
    if (vertices && vertexCount > 0)
    {
        void* temp = realloc(vertices, sizeof(Vertex) * vertexCount);
        if (!temp)
        {
            printf("Failed to reallocate vertices!\n");
            Delete();
            return 1;
        }
        if (vertices != temp) { free(vertices); }
        vertices = (Vertex*)temp;
        this->vertexCount = vertexCount;
    }
    else { CreateV(vertexCount); }

    if (faces && faceCount > 0)
    {
        void* temp = realloc(faces, sizeof(Face) * faceCount);
        if (!temp)
        {
            printf("Failed to reallocate faces!\n");
            Delete();
            return 1;
        }
        if (faces != temp) { free(faces); }
        faces = (Face*)temp;
        this->faceCount = faceCount;
    }
    else { CreateF(faceCount); }

    return 0;
}

int Mesh::RecreateV(int vertexCount)
{
    if (vertices && vertexCount > 0)
    {
        void* temp = realloc(vertices, sizeof(Vertex) * vertexCount);
        if (!temp)
        {
            printf("Failed to reallocate vertices!\n");
            Delete();
            return 1;
        }
        if (vertices != temp) { free(vertices); }
        vertices = (Vertex*)temp;
        this->vertexCount = vertexCount;
    }
    else { CreateV(vertexCount); }

    return 0;
}

int Mesh::RecreateF(int faceCount)
{
    if (faces && faceCount > 0)
    {
        void* temp = realloc(faces, sizeof(Face) * faceCount);
        if (!temp)
        {
            printf("Failed to reallocate faces!\n");
            Delete();
            return 1;
        }
        if (faces != temp) { free(faces); }
        faces = (Face*)temp;
        this->faceCount = faceCount;
    }
    else { CreateF(faceCount); }

    return 0;
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

void Mesh::Construct::Rectangle(Mesh& mesh, glm::vec2 position, glm::vec2 size)
{
    mesh.CreateV(4);
    Vertex* v = mesh.vertices;

    v[0] = Vertex{
        -size.x / 2.0f + position.x,
        -size.y / 2.0f + position.y,
        0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0
    };
    v[1] = Vertex{
        size.x / 2.0f + position.x,
        -size.y / 2.0f + position.y,
        0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0
    };
    v[2] = Vertex{
        size.x / 2.0f + position.x,
        size.y / 2.0f + position.y,
        0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0
    };
    v[3] = Vertex{
        -size.x / 2.0f + position.x,
        size.y / 2.0f + position.y,
        0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0
    };
}

void Mesh::Construct::Circle(Mesh& mesh, glm::vec2 position, float radius, int segments)
{
    mesh.CreateV(segments);
    Vertex* v = mesh.vertices;

    float angle = 0.0;
    const float dAngle = 2.0 * M_PI / (double)segments;
    for (int i = 0; i < segments; i++)
    {
        float x = radius * cos(angle) + position.x;
        float y = radius * sin(angle) + position.y;
        float tu = (cos(angle) + 1.0) / 2.0;
        float tv = (sin(angle) + 1.0) / 2.0;;

        v[i] = Vertex{
            x, y, 0.0, 1.0, 1.0, 1.0, 1.0, tu, tv
        };

        angle += dAngle;
    }
}

int Mesh::Bake::Triangles(Mesh& mesh)
{
    int faceCount = mesh.vertexCount - 2;
    if (faceCount <= 0) { return 1; }

    mesh.CreateF(faceCount);
    Face* f = mesh.faces;

    for (int i = 2; i < mesh.vertexCount; i++)
    {
        f[i - 2] = Face{
            0, i - 1, i,
            glm::vec3(0.0),
            -1
        };
    }
    
    return 0;
}

int Mesh::Bake::Lines(Mesh& mesh)
{
    int lineCount = mesh.vertexCount- 1;
    if (lineCount <= 0) { return 1; }

    mesh.CreateF(lineCount);
    Face* l = mesh.faces;

    for (int i = 1; i < mesh.vertexCount; i++)
    {
        l[i - 1] = Face{
            i - 1, i, i - 1,
            glm::vec3(0.0),
            -1
        };
    }

    return 0;
}

void Mesh::Modify::Colour(Mesh& mesh, glm::vec4 colour)
{
    Vertex* v = mesh.vertices;
    
    for (int i = 0; i < mesh.vertexCount; i++)
    {
        v[i].r = colour.r;
        v[i].g = colour.g;
        v[i].b = colour.b;
        v[i].a = colour.a;
    }
}

int Mesh::Modify::Append(Mesh& mesh, Mesh& add)
{
    Vertex* av = add.vertices;
    Face* af = add.faces;

    int base_vc = mesh.vertexCount;
    int base_fc = mesh.faceCount;
    int add_vc = add.vertexCount;
    int add_fc = add.faceCount;

    mesh.Recreate(base_vc + add_vc, base_fc + add_fc);

    Vertex* v = mesh.vertices;
    Face* f = mesh.faces;

    for (int i = 0; i < add_vc; i++)
    {
        v[base_vc + i] = Vertex{
            av[i].x, av[i].y, av[i].z,
            av[i].r, av[i].g, av[i].b, av[i].a,
            av[i].u, av[i].v,
        };
    }

    for (int i = 0; i < add_fc; i++)
    {
        f[base_fc + i] = Face{
            af[i].index[0] + base_vc,
            af[i].index[1] + base_vc,
            af[i].index[2] + base_vc,
            af[i].normal, -1
        };
    }

    return 0;
}
