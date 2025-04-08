#include "gframe.hpp"
#include "StartState.hpp"

void StartState::Init()
{
	//testMesh.Create(3, 1);
	//testMesh.vertices[0] = Vertex{ -0.5, -0.5, 0.0, 1.0, (float)0.8431372549, 0.0, 1.0, 0.0, 0.0 };
	//testMesh.vertices[1] = Vertex{  0.5, -0.5, 0.0, 1.0, (float)0.8431372549, 0.0, 1.0, 0.0, 0.0 };
	//testMesh.vertices[2] = Vertex{  0.0,  0.5, 0.0, 1.0, (float)0.8431372549, 0.0, 1.0, 0.0, 0.0 };
	//testMesh.faces[0] = Face{ 0, 1, 2, glm::vec3(0.0), -1 };

	/*Mesh circleMesh, rectMesh;
	Mesh::Construct::Circle(circleMesh, glm::vec2(0.0), 0.5, 5);
	Mesh::Modify::Colour(circleMesh, glm::vec4(1.0, (float)0.8431372549, 0.0, 1.0));
	Mesh::Construct::Rectangle(rectMesh, glm::vec2(0.0), glm::vec2(1.0));
	Mesh::Modify::Colour(rectMesh, glm::vec4(1.0, (float)0.8431372549, 0.0, 1.0));

	Mesh::Modify::Append(testMesh, circleMesh);
	Mesh::Modify::Append(testMesh, rectMesh);

	Mesh::Bake::Triangles(testMesh);*/

	Mesh::Construct::Circle(testMesh, glm::vec2(0.0), 0.5, 2000);
	Mesh::Bake::Triangles(testMesh);
	testMesh.Print();

	testShader.Read("vertex.glsl", GL_VERTEX_SHADER);    
	testShader.Read("fragment.glsl", GL_FRAGMENT_SHADER);
	testShader.Compile();

	printf("Start State initialised!\n");
}

void StartState::HandleInput()
{
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
