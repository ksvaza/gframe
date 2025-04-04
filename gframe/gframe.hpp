#pragma once

#include "AssetManager.hpp"
#include "InputManager.hpp"
#include "StateMachine.hpp"
#include "Window.hpp"
#include "renderer.hpp"
#include "Tools.hpp"
#include <iostream>
#include <memory>
#include <string>


struct GframeData
{
	Window window;
	Renderer Render;
	StateMachine Machine;
	InputManager Input;
	AssetManager AssetManager;
	Tools tools;
};

typedef std::shared_ptr<GframeData> GframeDataRef;
class Gframe
{
public:
	Gframe(int width, int height, std::string title);
private:
	const float dt = 1.0f / 60.0f;	
	GframeDataRef _data = std::make_shared<GframeData>();

	

	void Run();
};
