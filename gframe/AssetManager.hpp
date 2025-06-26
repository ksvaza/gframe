#pragma once

#include <map>
#include <string>
#include <vector>
#include "texture.hpp"
#include <fstream>
#include <sstream>
#include <array>
#include "mesh.hpp"
#include <unordered_map>
#include "shader.hpp"
#include "MeshComponent.hpp"

class AssetManager
{
public:

	AssetManager() {};
	~AssetManager() {};
	int8_t LoadTexture(std::string name, std::string FileName);
	Texture& GetTexture(std::string name);
	int8_t UnloadTexture(std::string name);

	

	
	int8_t LoadFont(std::string name, std::string FileName);
	Texture& GetFont(std::string name);
	int8_t UnloadFont(std::string name);

	void LoadSound(std::string name, std::string FileName);
	//sf::SoundBuffer& GetSound(std::string name);
	//void UnloadSound();


	int8_t LoadMeshFromOBJ(std::string name, std::string FileName);
	Mesh& GetMesh(std::string name);
	//MeshComponent& GetMeshComponent(std::string name);
	int8_t UnloadMesh(std::string name);

	void LoadAllMeshesFromFolder(std::string folderPath);

	int8_t LoadShader(std::string name, std::string vertexFile, std::string fragmentFile);
	Shader& GetShader(std::string name);
	int8_t UnloadShader(std::string name);

private:
	std::unordered_map <std::string, Texture> _textures;
	std::unordered_map <std::string, Mesh> _meshes;
	std::unordered_map<std::string, Shader> _shaders;
	//std::map <std::string, sf::Font> _fonts;
	//std::map <std::string, sf::SoundBuffer> _sounds;
};