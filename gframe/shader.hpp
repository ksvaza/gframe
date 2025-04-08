#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader
{
public:
//private:  // Shis ir pavisam nepareizi un uzreiz kaa straadaa failuceli shaderiem jaaatkomentee
	char* vertexShaderSrc = NULL;
	char* fragmentShaderSrc = NULL;
	GLuint vertexShaderID = 0;
	GLuint fragmentShaderID = 0;
	GLuint shaderProgramID = 0;
public:
	int Read(const char* filepath, GLenum type); // GL_VERTEX_SHADER vai GL_FRAGMENT_SHADER
	int Compile();
	GLuint GetID();
	void Use();
	void Delete();

	Shader();
	~Shader();
};
