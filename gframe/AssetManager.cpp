#include "AssetManager.hpp"
#include "stb_image.h"
#include <iostream>

int8_t  AssetManager::LoadTexture(std::string_view name, std::string_view FileName)
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
Texture& AssetManager::GetTexture(std::string_view name)
{
	static Texture ErrorTexture; // kaut kads error texture ka piem. pink and black error texture
	auto it = _textures.find(name);
	if (it != _textures.end())
	{
		return it->second;
	}
	return ErrorTexture;
}

int8_t AssetManager::UnloadTexture(std::string_view name)
{
	auto it = _textures.find(name);
	if (it != _textures.end())
	{
		stbi_image_free(it->second.data.raw);
		_textures.erase(it);
	}
	return -1;
}


int8_t AssetManager::LoadFont(std::string_view name, std::string_view FileName)
{
	return 0;
}