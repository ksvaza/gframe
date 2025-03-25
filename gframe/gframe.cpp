#include "gframe.hpp"
//#include "StartState.hpp"
#include <iostream>


Game::Game(int width, int height, std::string title)
{

	
	//_data->window.create(sf::VideoMode(width, height), title, sf::Style::Fullscreen);
	//_data->window.setMouseCursorVisible(0);
	//_data->Machine.AddState(StateRef(new StartState(_data)), 0);
	glfwSetTime();
	Run();
}

void Game::Run()
{
	
	float newTime, frameTime, interpolation;
	float currentTime = glfwGetTime();
	float accumulator = 0.0f;
	while (glfwWindowShouldClose(_data->window.))
	{
		_data->Machine.ProcessStateChanges();
		newTime = glfwGetTime();
		frameTime = newTime - currentTime;
		if (frameTime > 0.25f)
		{
			frameTime = 0.25f;
		}
		currentTime = newTime;
		accumulator += frameTime;
		while (accumulator >= dt)
		{
			_data->Machine.GetActiveState()->HandleInput();
			_data->Machine.GetActiveState()->Update(dt);
			accumulator -= dt;
		}
		interpolation = accumulator / dt;
		_data->Machine.GetActiveState()->Draw(interpolation);
	}
}