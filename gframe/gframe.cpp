#include "gframe.hpp"
#include "StartState.hpp"

Gframe::Gframe(int width, int height, std::string title)
{
    _data->window.Create(width, height, title);
    _data->Machine.AddState(StateRef(new StartState(_data)), 0);
    _data->Input.Initialise(_data->window.glWindow);

    glfwSetTime(0.0f);
    Run();
}

void Gframe::Run()
{
    const float tps = 60.0f;
    const float fps = 60.0f;
    float dt = 1.0f / 60.0f;

    float newTime, frameTime, interpolation;
    float currentTime = (float)glfwGetTime();
    float accumulator = 0.0f;

    while (!glfwWindowShouldClose(_data->window.glWindow))
    {
        _data->Machine.ProcessStateChanges();

        newTime = (float)glfwGetTime();
        frameTime = newTime - currentTime;

        if (frameTime > 0.25f)
        {
            frameTime = 0.25f;
        }

        currentTime = newTime;
        accumulator += frameTime;

        dt = frameTime;

        while (accumulator >= dt)
        {
            _data->Machine.GetActiveState()->HandleInput();
            _data->Machine.GetActiveState()->Update(dt);
            accumulator -= dt;
        }

        interpolation = accumulator / dt;

        _data->Machine.GetActiveState()->Draw(interpolation);

        _data->Input.Update();

        glfwSwapBuffers(_data->window.glWindow);
        glfwPollEvents();
    }
}
