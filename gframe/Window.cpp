#include "Window.hpp"
#include <stdio.h>
#include <stdlib.h>

int Window::Create(int width, int height, const char* title)
{
	// GLFW initialisation
	if (!glfwInit())
	{
		printf("Failed to initialise GLFW!\n");
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creating the window
	glWindow = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!glWindow)
	{
		printf("Failed to create window!\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(glWindow);

	// Resize callback
	glfwSetFramebufferSizeCallback(glWindow, framebuffer_size_callback_static);

	// vsync
	glfwSwapInterval(1);

	// GLAD initialisation
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialise GLAD!\n");
		glfwTerminate();
		return -1;
	}

	// Set window return pointer
	glfwSetWindowUserPointer(glWindow, this);

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, width, height);
	this->width = width;
	this->height = height;
	this->aspect_ratio = (float)width / (float)height;
	this->title = title;
	
	return 0;
}

Window::~Window()
{
	glfwTerminate();
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	this->width = width;
	this->height = height;
	this->aspect_ratio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}

void Window::framebuffer_size_callback_static(GLFWwindow* window, int width, int height)
{
	Window* w = (Window*)glfwGetWindowUserPointer(window);
	w->framebuffer_size_callback(window, width, height);
}
