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
#include "ecs.hpp"
#include "GframeData.hpp"


class Gframe
{
public:
	Gframe(int width, int height, std::string title);
private:
	const float dt = 1.0f / 60.0f;	
	GframeDataRef _data = std::make_shared<GframeData>();

	

	void Run();
};
