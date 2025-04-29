#include "gframe.hpp"
#include "StartState.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

void StartState::Init()
{
	//testMesh.Create(3, 1);
	//testMesh.vertices[0] = Vertex{ -0.5, -0.5, 0.0, 1.0, (float)0.8431372549, 0.0, 1.0, 0.0, 0.0 };
	//testMesh.vertices[1] = Vertex{  0.5, -0.5, 0.0, 1.0, (float)0.8431372549, 0.0, 1.0, 0.0, 0.0 };
	//testMesh.vertices[2] = Vertex{  0.0,  0.5, 0.0, 1.0, (float)0.8431372549, 0.0, 1.0, 0.0, 0.0 };
	//testMesh.faces[0] = Face{ 0, 1, 2, glm::vec3(0.0), -1 };

	Mesh circleMesh, rectMesh, lineMesh;

	Mesh::Construct::RegularPolygon(circleMesh, glm::vec3(0.0), 0.0, 0.6, 90);
	Mesh::Modify::Colour(circleMesh, glm::vec4(1.0, (float)0.8431372549, 0.0, 1.0));
	Mesh::Bake::Triangles(circleMesh);

	Mesh::Construct::Rectangle(rectMesh, glm::vec3(0.0), 0.0, glm::vec2(1.0));
	Mesh::Modify::Colour(rectMesh, glm::vec4(1.0, (float)0.8431372549, 0.0, 1.0));
	Mesh::Bake::TrianglesC(rectMesh);

	Mesh::Construct::Outline(lineMesh, circleMesh, 0.02);
	Mesh::Modify::Colour(lineMesh, glm::vec4(1.0, 1.0, 0.0, 1.0));
	Mesh::Bake::RectanglesC(lineMesh);

	Mesh::Modify::Append(testMesh, circleMesh);
	//Mesh::Modify::Append(testMesh, rectMesh);
	Mesh::Modify::Append(testMesh, lineMesh);

	//Mesh::Construct::Circle(testMesh, glm::vec2(0.0), 0.5, 2000);
	testMesh.Print();

	testShader.Read("vertex.glsl", GL_VERTEX_SHADER);    
	testShader.Read("fragment.glsl", GL_FRAGMENT_SHADER);
	testShader.Compile();

	batch.AddMesh(circleMesh);
	batch.AddMesh(rectMesh);
	batch.AddMesh(lineMesh);

	batch.OrderAndMapData();

	printf("Start State initialised!\n");
}

void StartState::HandleInput()
{
	printf("Inpt\n");
	if (Input.KeyDown(GLFW_KEY_E))
	{
		printf("Key Down 'E'\n");
		Mesh::Modify::Colour(testMesh, glm::vec4(0.0));
	}
	else if (Input.KeyUp(GLFW_KEY_E))
	{
		printf("Key Up 'E\n");
		Mesh::Modify::Colour(testMesh, glm::vec4(1.0));
	}

	if (Input.MouseScrollOffset().y > 0)
	{
		printf("Up\n");
	}
	else if (Input.MouseScrollOffset().y < 0)
	{
		printf("Down\n");
	}

	if (Input.ButtonDown(GLFW_MOUSE_BUTTON_LEFT))
	{
		printf("Spawn\n");
	}
}

void StartState::Update(float dt)
{

}

void StartState::Draw(float dt)
{
	printf("Draw\n");
	Render.Clear(glm::vec4(0.8, 0.0, 0.0, 1.0));
	//Render.DrawMesh(testMesh, testShader);
	batch.Draw(testShader);
	printf("Done \n");
}

void StartState::Pause()
{
}

void StartState::Resume()
{
}
