#include "gframe.hpp"
#include "StartState.hpp"

void StartState::Init()
{
	Input = _data->Input;
	Render = _data->Render;

	testMesh.Create(3, 1);
	testMesh.vertices[0] = Vertex{ -0.5, -0.5, 0.0, 1.0, (float)0.8431372549, 0.0, 1.0, 0.0, 0.0 };
	testMesh.vertices[1] = Vertex{  0.5, -0.5, 0.0, 1.0, (float)0.8431372549, 0.0, 1.0, 0.0, 0.0 };
	testMesh.vertices[2] = Vertex{  0.0,  0.5, 0.0, 1.0, (float)0.8431372549, 0.0, 1.0, 0.0, 0.0 };
	testMesh.faces[0] = Face{ 0, 1, 2, glm::vec3(0.0), -1 };

	//testShader.Read("vertex.glsl", GL_VERTEX_SHADER);      // Shis ir pavisam nepareizi un uzreiz kaa straadaa failuceli shaderiem jaaatkomentee
	//testShader.Read("fragment.glsl", GL_FRAGMENT_SHADER);  // Shis ir pavisam nepareizi un uzreiz kaa straadaa failuceli shaderiem jaaatkomentee
	std::string* vertexSrc = new std::string(
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec4 aCol;\n"
		"out vec4 oCol;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"   oCol = aCol;\n"
		"}\0"
	);
	std::string* fragmentSrc = new std::string(
		"#version 330 core\n"
		"in vec4 oCol;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = oCol;\n"
		"}\n\0"
	);
	testShader.vertexShaderSrc = (char*)vertexSrc->c_str();
	testShader.fragmentShaderSrc = (char*)fragmentSrc->c_str();
	testShader.Compile();

	printf("Start State initialised!\n");
}

void StartState::HandleInput()
{
	if (Input.KeyDown(GLFW_KEY_E))
	{
		printf("Key Down 'E'\n");
	}
	else if (Input.KeyUp(GLFW_KEY_E))
	{
		printf("Key Up 'E\n");
	}

	if (Input.MouseScrollOffset().y > 0)
	{
		printf("Up\n");
	}
	else if (Input.MouseScrollOffset().y < 0)
	{
		printf("Down\n");
	}
}

void StartState::Update(float dt)
{
	
}

void StartState::Draw(float dt)
{
	Render.Clear(glm::vec4(0.8, 0.0, 0.0, 1.0));
	Render.DrawMesh(testMesh, testShader);
}

void StartState::Pause()
{
}

void StartState::Resume()
{
}
