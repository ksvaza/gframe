#pragma once
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window() {};
	~Window();
	
	GLFWwindow* glWindow = NULL;
	int width = 0;
	int height = 0;
	float aspect_ratio = 0.0f;
	const char* title = NULL;

	int Create(int width, int height, const char* title);
private:
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void framebuffer_size_callback_static(GLFWwindow* window, int width, int height);
};