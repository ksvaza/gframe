#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aCol;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 oCol;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    //gl_Position = model * vec4(aPos, 1.0);
    oCol = aCol;
}