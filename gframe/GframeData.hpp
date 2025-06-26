#pragma once
#include "AssetManager.hpp"
#include "InputManager.hpp"
#include "StateMachine.hpp"
#include "Window.hpp"
#include "renderer.hpp"
#include "ecs.hpp"
#include "Camera.hpp"


struct GframeData
{
	Window window;
	Renderer Render;
	StateMachine Machine;
	InputManager Input;
	AssetManager AssetManager;
	ECS::ecs ecs;
	Camera camera;
};

typedef std::shared_ptr<GframeData> GframeDataRef;