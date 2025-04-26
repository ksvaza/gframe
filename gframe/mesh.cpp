#include "mesh.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "mesh.hpp"
#include "shader.hpp"
#include <vector>

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

void Mesh::UploadToGPU()
{
    if (uploaded) return;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    std::vector<unsigned int> indices;
    for (int i = 0; i < faceCount; i++)
    {
        indices.push_back(faces[i].index[0]);
        indices.push_back(faces[i].index[1]);
        indices.push_back(faces[i].index[2]);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    uploaded = true;
}



int Mesh::Create(int vertexCount, int faceCount)
{
    int ret_val = 0;
    ret_val = ret_val | CreateV(vertexCount);
    ret_val = ret_val | CreateF(faceCount);
    return ret_val;
}

int Mesh::CreateV(int vertexCount)
{
    if (vertexCount <= 0)
    {
        return 0;
    }
    if (vertices) { free(vertices); vertices = NULL; }
    vertices = (Vertex*)malloc(sizeof(Vertex) * vertexCount);
    if (!vertices)
    {
        printf("Failed to allocate vertices!\n");
        Delete();
        return 1;
    }
    this->vertexCount = vertexCount;

    return 0;
}

int Mesh::CreateF(int faceCount)
{
    if (faceCount <= 0)
    {
        return 0;
    }
    if (faces) { free(faces); faces = NULL; }
    faces = (Face*)malloc(sizeof(Face) * faceCount);
    if (!faces)
    {
        printf("Failed to allocate faces!\n");
        Delete();
        return 1;
    }
    this->faceCount = faceCount;

    return 0;
}

int Mesh::Recreate(int vertexCount, int faceCount)
{
    RecreateV(vertexCount);
    RecreateF(faceCount);

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
        "\tScale: (" + std::to_string(transform.scale.x) + ", " + std::to_string(transform.scale.y) + ", " + std::to_string(transform.scale.z) + ")\n}\n"
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

void Mesh::Construct::Rectangle(Mesh& mesh, glm::vec3 position, float angle, glm::vec2 size)
{
    mesh.CreateV(4);
    Vertex* v = mesh.vertices;

    // Construct the rectangle
    v[0] = Vertex{
        -size.x / 2.0f,
        -size.y / 2.0f,
        position.z,
        1.0, 1.0, 1.0, 1.0, 0.0, 0.0
    };
    v[1] = Vertex{
        size.x / 2.0f,
        -size.y / 2.0f,
        position.z,
        1.0, 1.0, 1.0, 1.0, 1.0, 0.0
    };
    v[2] = Vertex{
        size.x / 2.0f,
        size.y / 2.0f,
        position.z,
        1.0, 1.0, 1.0, 1.0, 1.0, 1.0
    };
    v[3] = Vertex{
        -size.x / 2.0f,
        size.y / 2.0f,
        position.z,
        1.0, 1.0, 1.0, 1.0, 0.0, 1.0
    };

    // Rotate the rectangle about the origin and move to position
    v[0].x = v[0].x * cos(angle) - v[0].y * sin(angle) + position.x;
    v[0].y = v[0].x * sin(angle) + v[0].y * cos(angle) + position.y;
    v[1].x = v[1].x * cos(angle) - v[1].y * sin(angle) + position.x;
    v[1].y = v[1].x * sin(angle) + v[1].y * cos(angle) + position.y;
    v[2].x = v[2].x * cos(angle) - v[2].y * sin(angle) + position.x;
    v[2].y = v[2].x * sin(angle) + v[2].y * cos(angle) + position.y;
    v[3].x = v[3].x * cos(angle) - v[3].y * sin(angle) + position.x;
    v[3].y = v[3].x * sin(angle) + v[3].y * cos(angle) + position.y;
}

void Mesh::Construct::RegularPolygon(Mesh& mesh, glm::vec3 position, float angle, float radius, int segments)
{
    mesh.CreateV(segments);
    Vertex* v = mesh.vertices;

    float aAngle = angle;
    const float dAngle = 2.0 * M_PI / (double)segments;
    for (int i = 0; i < segments; i++)
    {
        float x = radius * cos(aAngle) + position.x;
        float y = radius * sin(aAngle) + position.y;
        float tu = (cos(aAngle) + 1.0) / 2.0;
        float tv = (sin(aAngle) + 1.0) / 2.0;;

        v[i] = Vertex{
            x, y, position.z, 1.0, 1.0, 1.0, 1.0, tu, tv
        };

        aAngle += dAngle;
    }
}

void Mesh::Construct::Line(Mesh& mesh, glm::vec3 start, glm::vec3 end, float width)
{
    mesh.CreateV(4);
    Vertex* v = mesh.vertices;

    float dst = glm::distance(glm::vec2(start), glm::vec2(end));
    glm::vec2 dif = glm::vec2(end - start);

    // Construct points of the line as a rectangle
    v[0] = Vertex{
        (-dif.y * width) / (dst * 2.0f) + start.x,
        (dif.x * width) / (dst * 2.0f) + start.y,
        start.z,
        1.0, 1.0, 1.0, 1.0, 0.0, 0.0
    };
    v[1] = Vertex{
        (-dif.y * width) / (dst * 2.0f) + end.x,
        (dif.x * width) / (dst * 2.0f) + end.y,
        end.z,
        1.0, 1.0, 1.0, 1.0, 0.0, 0.0
    };
    v[2] = Vertex{
        (dif.y * width) / (dst * 2.0f) + end.x,
        (-dif.x * width) / (dst * 2.0f) + end.y,
        end.z,
        1.0, 1.0, 1.0, 1.0, 0.0, 0.0
    };
    v[3] = Vertex{
        (dif.y * width) / (dst * 2.0f) + start.x,
        (-dif.x * width) / (dst * 2.0f) + start.y,
        start.z,
        1.0, 1.0, 1.0, 1.0, 0.0, 0.0
    };
}

void Mesh::Construct::Lines(Mesh& mesh, glm::vec3* endpoints, int pointcount, float width)
{
    if (pointcount < 1) { return; }
    mesh.CreateV(pointcount * 2);
    Vertex* v = mesh.vertices;

    float sDst = glm::distance(glm::vec2(endpoints[0]), glm::vec2(endpoints[1]));
    glm::vec2 sDif = glm::vec2(endpoints[0] - endpoints[1]);

    // Construct start points
    v[0] = Vertex{
        (-sDif.y * width) / (sDst * 2.0f) + endpoints[0].x,
        (sDif.x * width) / (sDst * 2.0f) + endpoints[0].y,
        endpoints[0].z,
        1.0, 1.0, 1.0, 1.0, 0.0, 0.0
    };
    v[1] = Vertex{
        (sDif.y * width) / (sDst * 2.0f) + endpoints[0].x,
        (-sDif.x * width) / (sDst * 2.0f) + endpoints[0].y,
        endpoints[0].z,
        1.0, 1.0, 1.0, 1.0, 0.0, 0.0
    };

    for (int i = 1; i < pointcount - 1; i++)
    {
        float dst = glm::distance(glm::vec2(endpoints[i - 1]), glm::vec2(endpoints[i + 1]));
        glm::vec2 dif = glm::vec2(endpoints[i - 1] - endpoints[i + 1]);

        // Construct points
        v[i * 2] = Vertex{
            (-dif.y * width) / (dst * 2.0f) + endpoints[i].x,
            (dif.x * width) / (dst * 2.0f) + endpoints[i].y,
            endpoints[i].z,
            1.0, 1.0, 1.0, 1.0, 0.0, 0.0
        };
        v[i * 2 + 1] = Vertex{
            (dif.y * width) / (dst * 2.0f) + endpoints[i].x,
            (-dif.x * width) / (dst * 2.0f) + endpoints[i].y,
            endpoints[i].z,
            1.0, 1.0, 1.0, 1.0, 0.0, 0.0
        };
    }

    float eDst = glm::distance(glm::vec2(endpoints[pointcount - 2]), glm::vec2(endpoints[pointcount - 1]));
    glm::vec2 eDif = glm::vec2(endpoints[pointcount - 2] - endpoints[pointcount - 1]);

    // Construct start points
    v[pointcount * 2 - 2] = Vertex{
        (-eDif.y * width) / (eDst * 2.0f) + endpoints[pointcount - 1].x,
        (eDif.x * width) / (eDst * 2.0f) + endpoints[pointcount - 1].y,
        endpoints[pointcount - 1].z,
        1.0, 1.0, 1.0, 1.0, 0.0, 0.0
    };
    v[pointcount * 2 - 1] = Vertex{
        (eDif.y * width) / (eDst * 2.0f) + endpoints[pointcount - 1].x,
        (-eDif.x * width) / (eDst * 2.0f) + endpoints[pointcount - 1].y,
        endpoints[pointcount - 1].z,
        1.0, 1.0, 1.0, 1.0, 0.0, 0.0
    };
}

void Mesh::Construct::Outline(Mesh& mesh, Mesh& source, float width)
{
    int c = source.vertexCount;
    glm::vec3* p = (glm::vec3*)malloc(sizeof(glm::vec3) * (c + 1));
    if (!p) { return; }
    for (int i = 0; i < c; i++)
    {
        p[i] = glm::vec3(source.vertices[i].x, source.vertices[i].y, source.vertices[i].z);
    }
    p[c] = glm::vec3(source.vertices[0].x, source.vertices[0].y, source.vertices[0].z);
    Lines(mesh, p, c + 1, width);
    free(p);
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

int Mesh::Bake::TrianglesC(Mesh& mesh)
{
    int faceCount = mesh.vertexCount / 2;
    if (faceCount <= 0) { return 1; }

    mesh.CreateF(faceCount);
    Face* f = mesh.faces;

    for (int i = 0; i < mesh.faceCount; i++)
    {
        f[i] = Face{
            i * 2, i * 2 + 1, i * 2 + 2,
            glm::vec3(0.0),
            -1
        };
        if (i * 2 + 2 == mesh.vertexCount)
        {
            f[i].index[2] = 0;
        }
    }

    return 0;
}

int Mesh::Bake::RectanglesC(Mesh& mesh)
{
    if (mesh.vertexCount < 4) { return 1; }
    int faceCount = mesh.vertexCount - 2;
    if (faceCount <= 0) { return 1; }

    mesh.CreateF(faceCount);
    Face* f = mesh.faces;

    for (int i = 0; i < mesh.faceCount / 2; i++)
    {
        f[i * 2] = Face{
            i * 2, i * 2 + 2, i * 2 + 1,
            glm::vec3(0.0),
            -1
        };
        if (i * 2 + 1 < mesh.faceCount)
        {
            f[i * 2 + 1] = Face{
                i * 2 + 1, i * 2 + 2, i * 2 + 3,
                glm::vec3(0.0),
                -1
            };
        }
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
