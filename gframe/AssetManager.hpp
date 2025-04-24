#pragma once

#include <map>
#include <string>
#include <vector>
#include "texture.hpp"

class AssetManager
{
public:

	AssetManager() {};
	~AssetManager() {};
	int8_t LoadTexture(std::string_view name, std::string_view FileName);
	Texture& GetTexture(std::string_view name);
	int8_t UnloadTexture(std::string_view name);
	
	int8_t LoadFont(std::string_view name, std::string_view FileName);
	Texture& GetFont(std::string_view name);
	int8_t UnloadFont(std::string_view name);

	void LoadSound(std::string name, std::string FileName);
	//sf::SoundBuffer& GetSound(std::string name);
	//void UnloadSound();

private:
	std::map <std::string_view, Texture> _textures;
	//std::map <std::string, sf::Font> _fonts;
	//std::map <std::string, sf::SoundBuffer> _sounds;
};
