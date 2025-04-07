#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string_view>

struct Vertex
{
	float x = 0.0, y = 0.0, z = 0.0;
	float r = 0.0, g = 0.0, b = 0.0, a = 0.0;
	float u = 0.0, v = 0.0;
};

struct Face
{
	int index[3] = { 0, 0, 0 };
	glm::vec3 normal = glm::vec3(0.0);
	int textureID = -1;
};

struct TextureRef
{
	std::string_view texName;
	// Buutu veel jaauzraksta AssetManager, lai sheit vareetu pabeigt liidz galam.
};