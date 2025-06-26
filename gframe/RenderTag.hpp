#pragma once
#include <string>

class RenderTag 
{
public:
	RenderTag() = default;
	RenderTag(std::string shaderKey) : ShaderKey(shaderKey) {
		std::cout << "hello " << ShaderKey << '\n';
	}
	std::string ShaderKey;
};