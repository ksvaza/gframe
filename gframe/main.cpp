#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "InputManager.hpp"
#include "Window.hpp"
#include "gframe.hpp"

int main(void)
{
    Gframe game(800, 800, "Hello my goofy ahh comrades!");
    
 //   Window window;
	//window.Create(800, 600, "Hello World");

 //   /* Loop until the user closes the window */
 //   while (!glfwWindowShouldClose(window.glWindow))
 //   {
 //       /* Render here */
 //       glClear(GL_COLOR_BUFFER_BIT);

 //       /* Swap front and back buffers */
 //       glfwSwapBuffers(window.glWindow);

 //       /* Poll for and process events */
 //       glfwPollEvents();
 //   }

    return 0;
}