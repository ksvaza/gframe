#include "gframe.hpp"
#include "StartState.hpp"


Game::Game(int width, int height, const char* title)
{

	
	//_data->window.create(sf::VideoMode(width, height), title, sf::Style::Fullscreen);
	//_data->window.setMouseCursorVisible(0);
	_data->window.Create(width, height, title);
	_data->Machine.AddState(StateRef(new StartState(_data)), 0);
	InputManager::HWInputs::Initialise(_data->window.glWindow);
	
	glfwSetTime(0.0f);
	Run();
}

void Game::Run()
{
	
	float newTime, frameTime, interpolation;
	float currentTime = glfwGetTime();
	float accumulator = 0.0f;
	while (!glfwWindowShouldClose(_data->window.glWindow))
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

		InputManager::HWInputs::Update();

		glfwSwapBuffers(_data->window.glWindow);
		glfwPollEvents();
	}
}