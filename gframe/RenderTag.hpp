#pragma once
#include <string>

class RenderTag 
{
public:
	RenderTag() = default;
	RenderTag(const char* shaderKey)
	{
#ifdef _MSC_VER
		strncpy_s(ShaderKey, 64, shaderKey, _TRUNCATE);
#else
		std::strncpy(dest, src, 63);
		dest[63] = '\0';
#endif
	}
	char ShaderKey[64];
};