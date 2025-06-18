#include "gframe.hpp"
#include "StartState.hpp"
#include <vector>

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
    glfwMakeContextCurrent(_data->window.glWindow);
    glfwSwapInterval(2);

    const float tps = 60.0f;
    const float dt = 1.0f / tps;
    float accumulator = 0.0f;
    float currentTime = (float)glfwGetTime();


    float newTime, frameTime, interpolation;

    while (!glfwWindowShouldClose(_data->window.glWindow))
    {
        _data->Machine.ProcessStateChanges();

        newTime = (float)glfwGetTime();
        frameTime = newTime - currentTime;


        static float fpsTimer = 0.0f;
        static int frameCount = 0;

        frameCount++;
        fpsTimer += frameTime;

        if (fpsTimer >= 1.0f) {
            std::cout << "FPS: " << frameCount << '\n';
            frameCount = 0;
            fpsTimer = 0.0f;
        }

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

        _data->Input.Update();

        glfwSwapBuffers(_data->window.glWindow);
        glfwPollEvents();
    }
}
