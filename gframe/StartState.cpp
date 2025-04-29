#include "gframe.hpp"
#include "StartState.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include "texture.hpp"

void StartState::Init()
{
	_data->AssetManager.LoadTexture("check", "C:/Users/jekabins/Downloads/test.png");
	//_data->AssetManager.UnloadTexture("check");

	Pixel3* pixels = _data->AssetManager.GetTexture("check").data.ch3;
	std::cout << _data->AssetManager.GetTexture("check").nrChannels << '\n' << '\n';
	//for (int i = 0; i < _data->AssetManager.GetTexture("check").width * _data->AssetManager.GetTexture("check").height;i++)
	//{
	//	/*std::cout << "Pixel " << i << ": "
	//		<< +pixels[i].r << ", "
	//		<< +pixels[i].g << ", "
	//		<< +pixels[i].b << ", "
	//		<< +pixels[i].a << "\n";*/
	//	std::cout << "Pixel " << i << ": "
	//		<< +pixels[i].r << ", "
	//		<< +pixels[i].g << ", "
	//		<< +pixels[i].b << "\n";
	//}


	//testMesh.Create(3, 1);
	//testMesh.vertices[0] = Vertex{ -0.5, -0.5, 0.0, 1.0, (float)0.8431372549, 0.0, 1.0, 0.0, 0.0 };
	//testMesh.vertices[1] = Vertex{  0.5, -0.5, 0.0, 1.0, (float)0.8431372549, 0.0, 1.0, 0.0, 0.0 };
	//testMesh.vertices[2] = Vertex{  0.0,  0.5, 0.0, 1.0, (float)0.8431372549, 0.0, 1.0, 0.0, 0.0 };
	//testMesh.faces[0] = Face{ 0, 1, 2, glm::vec3(0.0), -1 };


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
	Mesh::Modify::Append(testMesh, rectMesh);
	Mesh::Modify::Append(testMesh, lineMesh);

	testMesh.transform.scale = glm::vec3(1.0);
	//Mesh::Construct::Circle(testMesh, glm::vec2(0.0), 0.5, 2000);
	testMesh.Print();

	testShader.Read("vertex.glsl", GL_VERTEX_SHADER);    
	testShader.Read("fragment.glsl", GL_FRAGMENT_SHADER);
	testShader.Compile();

	printf("Batch startup\n");

	batch = Renderer::Batch();
	batch.AddMesh(circleMesh);
	batch.AddMesh(rectMesh);
	batch.AddMesh(lineMesh);

	//batch.OrderAndMapData();

	printf("Start State initialised!\n");
}

void StartState::HandleInput()
{
	if (Input.Key(GLFW_KEY_W))
	{
		testMesh.transform.position.y += 0.01;
	}
	if (Input.Key(GLFW_KEY_S))
	{
		testMesh.transform.position.y -= 0.01;
	}
	if (Input.Key(GLFW_KEY_A))
	{
		testMesh.transform.position.x -= 0.01;
	}
	if (Input.Key(GLFW_KEY_D))
	{
		testMesh.transform.position.x += 0.01;
	}


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
	//testMesh.transform.position.x += 0;
	//std::cout << testMesh.transform.position.x << ' ';
}

void StartState::Draw(float dt)
{
	Render.Clear(glm::vec4(0.8, 0.0, 0.0, 1.0));
	//Render.DrawMesh(testMesh, testShader);
	printf("Draw\n");
	batch.Draw(testShader);
	printf("Done \n");
}

void StartState::Pause()
{
}

void StartState::Resume()
{
}
