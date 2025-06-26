#define _CRT_SECURE_NO_WARNINGS
#include "AssetManager.hpp"
#include "stb_image.h"
#include <filesystem>
#include <iostream>
#include <stdio.h>
#include <glad/glad.h>

namespace fs = std::filesystem;

int8_t  AssetManager::LoadTexture(std::string name, std::string FileName)
{
	int Width, Height, nrChannels;
	Texture texture;
	texture.data.raw = stbi_load(FileName.data(), &Width, &Height, &nrChannels, 0);
	if (!texture.data.raw)
	{
		std::cout << "Failed to load texture from:\n" << FileName << '\n';


		//printf("Failed to load texture %s\n", FileName.data());
		return -1;
	}
	texture.height = Height;
	texture.width = Width;
	texture.nrChannels = nrChannels;
	_textures[name] = texture;
	return 0;
}
Texture& AssetManager::GetTexture(std::string name)
{
	static Texture ErrorTexture; // kaut kads error texture ka piem. pink and black error texture
	auto it = _textures.find(name);
	if (it != _textures.end())
	{
		return it->second;
	}
	return ErrorTexture;
}
int8_t AssetManager::UnloadTexture(std::string name)
{
	auto it = _textures.find(name);
	if (it != _textures.end())
	{
		stbi_image_free(it->second.data.raw);
		_textures.erase(it);
	}
	return -1;
}

int8_t AssetManager::LoadMeshFromOBJ(std::string name, std::string fileName)
{
    
    Mesh mesh;
    std::vector<Vertex> tempVertices;
    std::vector<Face> tempFaces;
    std::vector<TextureRef> tempTextureRefs;

    std::ifstream file(fileName.data());
    if (!file.is_open())
    {
        std::cout << "Failed to open OBJ file: " << fileName << '\n';
        return -1;
    }

    std::vector<glm::vec3> positions(1, glm::vec3(0.0f));
    std::vector<glm::vec2> uvs(1, glm::vec2(0.0f));
    std::vector<glm::vec3> normals(1, glm::vec3(0.0f));

    std::string line;
    int currentTextureID = -1;

    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        const char* cline = line.c_str();

        if (cline[0] == 'v' && cline[1] == ' ')
        {
            glm::vec3 pos;
            sscanf(cline + 2, "%f %f %f", &pos.x, &pos.y, &pos.z);
            positions.push_back(pos);
        }
        else if (cline[0] == 'v' && cline[1] == 't')
        {
            glm::vec2 uv;
            sscanf(cline + 3, "%f %f", &uv.x, &uv.y);
            uvs.push_back(uv);
        }
        else if (cline[0] == 'v' && cline[1] == 'n')
        {
            glm::vec3 normal;
            sscanf(cline + 3, "%f %f %f", &normal.x, &normal.y, &normal.z);
            normals.push_back(normal);
        }
        else if (line.rfind("usemtl ", 0) == 0)
        {
            TextureRef texRef;
            texRef.texName = std::string_view(cline + 7);
            tempTextureRefs.push_back(texRef);
            currentTextureID = static_cast<int>(tempTextureRefs.size()) - 1;
        }
        else if (cline[0] == 'f' && cline[1] == ' ')
        {
            Face face;
            face.textureID = currentTextureID;

            int v[3] = {}, t[3] = {}, n[3] = {};
            int matches = sscanf(cline + 2, "%d/%d/%d %d/%d/%d %d/%d/%d",
                &v[0], &t[0], &n[0],
                &v[1], &t[1], &n[1],
                &v[2], &t[2], &n[2]);

            if (matches < 9)
            {
                // Faces without uvs/normals
                sscanf(cline + 2, "%d %d %d", &v[0], &v[1], &v[2]);
            }

            for (int i = 0; i < 3; ++i)
            {
                glm::vec3 pos = positions[v[i]];
                glm::vec2 uv = (t[i] < (int)uvs.size()) ? uvs[t[i]] : glm::vec2(0.0f);
                glm::vec3 normal = (n[i] < (int)normals.size()) ? normals[n[i]] : glm::vec3(0.0f);

                Vertex vert;
                vert.x = pos.x;
                vert.y = pos.y;
                vert.z = pos.z;
                vert.u = uv.x;
                vert.v = uv.y;

                tempVertices.push_back(vert);
                face.index[i] = static_cast<int>(tempVertices.size()) - 1;
            }

            if (normals.size() <= 1)
            {
                glm::vec3 a(tempVertices[face.index[0]].x, tempVertices[face.index[0]].y, tempVertices[face.index[0]].z);
                glm::vec3 b(tempVertices[face.index[1]].x, tempVertices[face.index[1]].y, tempVertices[face.index[1]].z);
                glm::vec3 c(tempVertices[face.index[2]].x, tempVertices[face.index[2]].y, tempVertices[face.index[2]].z);
                face.normal = glm::normalize(glm::cross(b - a, c - a));
            }
            else
            {
                face.normal = normals[n[0]];
            }

            tempFaces.push_back(face);
        }
    }

    file.close();

    mesh.Create(static_cast<int>(tempVertices.size()), static_cast<int>(tempFaces.size()));

    for (int i = 0; i < mesh.vertexCount; i++)
        mesh.vertices[i] = tempVertices[i];

    for (int i = 0; i < mesh.faceCount; i++)
        mesh.faces[i] = tempFaces[i];

    mesh.textureRefCount = static_cast<int>(tempTextureRefs.size());
    if (mesh.textureRefCount > 0)
    {
        mesh.textureReferences = new TextureRef[mesh.textureRefCount];
        for (int i = 0; i < mesh.textureRefCount; ++i)
            mesh.textureReferences[i] = tempTextureRefs[i];
    }
    //std::cout << mesh;
    //std::cout << name << '|' << '\n';
    
    _meshes[name] = mesh;
    GetMesh("cube");
    //_meshes.emplace(name, std::move(mesh));
    return 0;
}
Mesh& AssetManager::GetMesh(std::string name)
{
    static Mesh ErrorMesh; // Kaut kads error mesh
    auto it = _meshes.find(name);
    if (it != _meshes.end())
    {
        return it->second;
    }
    return ErrorMesh;
}
/*MeshComponent& AssetManager::GetMeshComponent(std::string name)
{
    static Mesh ErrorMesh; // Kaut kads error mesh
    MeshComponent mesh;
    auto it = _meshes.find(name);
    if (it != _meshes.end())
    {
        mesh.colour = it->second.colour;
        mesh.VAO = it->second.VAO;
        mesh.VBO = it->second.VBO;
        mesh.EBO = it->second.EBO;
        mesh.isUploaded = it->second.uploaded;
        mesh.vertices = it->second.vertices;
        mesh.vertexCount = it->second.vertexCount;
        mesh.faces = it->second.faces;
        mesh.faceCount = it->second.faceCount;
        mesh.textureReferences = it->second.textureReferences;
        mesh.textureRefCount = it->second.textureRefCount;
        return mesh;
    }
    mesh.colour = ErrorMesh.colour;
    mesh.VAO = ErrorMesh.VAO;
    mesh.VBO = ErrorMesh.VBO;
    mesh.EBO = ErrorMesh.EBO;
    mesh.isUploaded = ErrorMesh.uploaded;
    mesh.vertices = ErrorMesh.vertices;
    mesh.vertexCount = ErrorMesh.vertexCount;
    mesh.faces = ErrorMesh.faces;
    mesh.faceCount = ErrorMesh.faceCount;
    mesh.textureReferences = ErrorMesh.textureReferences;
    mesh.textureRefCount = ErrorMesh.textureRefCount;
    return mesh;
}*/
int8_t AssetManager::UnloadMesh(std::string name)
{
    auto it = _meshes.find(name);
    if (it != _meshes.end())
    {
        it->second.Delete();

        if (it->second.uploaded)
        {
            glDeleteVertexArrays(1, &it->second.VAO);
            glDeleteBuffers(1, &it->second.VBO);
            glDeleteBuffers(1, &it->second.EBO);
        }

        _meshes.erase(it);
        return 0;
    }

    return -1; 
}



void AssetManager::LoadAllMeshesFromFolder(std::string folderPath)
{
    namespace fs = std::filesystem;
    if (!fs::exists(folderPath))
    {
        std::cout << "Folder does not exist: " << folderPath << '\n';
        return;
    }

    if (!fs::is_directory(folderPath))
    {
        std::cout << "Path is not a directory: " << folderPath << '\n';
        return;
    }

    for (const auto& entry : fs::directory_iterator(folderPath))
    {
        if (entry.is_regular_file())
        {
            const fs::path& filePath = entry.path();
            if (filePath.extension() == ".obj")
            {
                std::string fileName = filePath.filename().string();
                std::string fullPath = filePath.string();
                std::replace(fullPath.begin(), fullPath.end(), '\\', '/');

                std::string meshName = filePath.stem().string();

                if (LoadMeshFromOBJ(meshName, fullPath) != 0)
                {
                    std::cout << "Failed to load mesh: " << fullPath << "\n";
                }
                else
                {
                    std::cout << "Successfully loaded: " << fullPath << "\n";
                    
                }
                
            }
        }
    }
}

int8_t AssetManager::LoadShader(std::string name, std::string vertexFile, std::string fragmentFile)
{
    Shader shader;
    shader.Read(vertexFile.c_str(), GL_VERTEX_SHADER);
    shader.Read(fragmentFile.c_str(), GL_FRAGMENT_SHADER);
    shader.Compile();
    _shaders[name] = shader;
    return 0;
}

Shader& AssetManager::GetShader(std::string name)
{
    static Shader ErrorShader; // Kaut kads error shader
    auto it = _shaders.find(name);
    if (it != _shaders.end())
	{
		return it->second;
	}
    return ErrorShader;
}

int8_t AssetManager::UnloadShader(std::string name)
{
    auto it = _shaders.find(name);
    if (it != _shaders.end())
	{
		it->second.Delete();
		_shaders.erase(it);
		return 0;
	}
    return -1;
    return 0;
}







int8_t AssetManager::LoadFont(std::string name, std::string FileName)
{
	return 0;
}


