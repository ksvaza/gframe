#pragma once

#include "AssetManager.hpp"
#include "InputManager.hpp"
#include "StateMachine.hpp"
#include "Window.hpp"
#include "Tools.hpp"
#include <iostream>
#include <memory>
#include <string>


struct GameData
{
	Window window;
	StateMachine Machine;
	InputManager inputManager;
	AssetManager AssetManager;
	Tools tools;
};

typedef std::shared_ptr<GameData> GameDataRef;
class Game
{
public:
	Game(int width, int height, const char* title);
private:
	const float dt = 1.0f / 60.0f;	
	GameDataRef _data = std::make_shared<GameData>();

	

	void Run();
};
