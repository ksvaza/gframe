#include "gframe.hpp"
#include "StartState.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include "texture.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "Velocity.hpp"
#include "InputTag.hpp"
#include "MovementSystem.hpp"
#include "Movement.hpp"
#include "TransformComponent.hpp"
#include "RenderTag.hpp"
#include "MeshComponent.hpp"
#include"RenderSystem.hpp"


void StartState::Init()
{
	player = _data->ecs.CreateEntity();
	cube = _data->ecs.CreateEntity();
	_data->ecs.RegisterComponent<Velocity>();
	_data->ecs.RegisterComponent<InputTag>();
	_data->ecs.RegisterComponent<PlayerTag>();
	_data->ecs.RegisterComponent<TransformComponent>();
	_data->ecs.RegisterComponent<RenderTag>();
	_data->ecs.RegisterComponent<MeshComponent>();

	_data->ecs.AddComponent<Velocity>(player, Velocity(0.0f, 0.0f, 0.0f ));
	_data->ecs.AddComponent<InputTag>(player, InputTag());
	_data->ecs.AddComponent<PlayerTag>(player, PlayerTag());
	_data->ecs.AddComponent<TransformComponent>(player, TransformComponent());
	_data->ecs.AddComponent<RenderTag>(player, RenderTag("testShader"));

	_data->ecs.AddComponent<TransformComponent>(cube, TransformComponent(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
	_data->ecs.AddComponent<RenderTag>(cube, RenderTag("testShader"));


	_data->ecs.RegisterSystem(MovementSystem);
	_data->ecs.RegisterSystem(UpdateMovement);
	_data->ecs.RegisterSystem(RenderSystem);
	yaw = -90.0f;
	pitch = 0;
	//CameraPos = glm::vec3(0.0f, 0.0f, 30.0f);

	_data->AssetManager.LoadAllMeshesFromFolder("../StartState");


	//_data->ecs.AddComponent<MeshComponent>(player, MeshComponent(_data->AssetManager.GetMeshComponent("cube")));
	_data->ecs.AddComponent<MeshComponent>(cube, MeshComponent(_data->AssetManager.GetMeshComponent("cube")));


	Pixel3* pixels = _data->AssetManager.GetTexture("check").data.ch3;

	testMesh2 = _data->AssetManager.GetMesh("cube");
	//std::cout << testMesh2;

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

	testMesh.transform.scale = glm::vec3(1.0);
	//Mesh::Construct::Circle(testMesh, glm::vec2(0.0), 0.5, 2000);
	//testMesh.Print();

	std::cout << testMesh2;
	testMesh2.transform.scale = glm::vec3(1.0f);

	_data->AssetManager.LoadShader("testShader", "vertex.glsl", "fragment.glsl");
	std::cout << "shader ID: " << _data->AssetManager.GetShader("testShader").GetID() << '\n';

	testShader.Read("vertex.glsl", GL_VERTEX_SHADER);    
	testShader.Read("fragment.glsl", GL_FRAGMENT_SHADER);
	testShader.Compile();

	printf("Start State initialised!\n");
}

void StartState::HandleInput()
{
	/*float sensitivity = 0.1f;
	float speed = 1.0f;
	if (Input.Button(0))
	{
		yaw += Input.MouseDeltaPosition().x * sensitivity;
		pitch -= Input.MouseDeltaPosition().y * sensitivity;

		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;

		glfwSetInputMode(_data->window.glWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	else
		glfwSetInputMode(_data->window.glWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	



	if (Input.Key(GLFW_KEY_W))
	{
		CameraPos.z -= speed;
	}
	if (Input.Key(GLFW_KEY_S))
	{
		CameraPos.z += speed;
	}
	if (Input.Key(GLFW_KEY_A))
	{
		CameraPos.x -= speed;
	}
	if (Input.Key(GLFW_KEY_D))
	{
		CameraPos.x += speed;
	}
	if (Input.Key(GLFW_KEY_SPACE))
	{
		CameraPos.y += speed;
	}
	if (Input.Key(GLFW_KEY_LEFT_SHIFT))
	{
		CameraPos.y -= speed;
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
	}*/
}

void StartState::Update(float dt)
{
	_data->ecs.UpdateSystems(_data.get(), dt);
	//Velocity* vel = _data->ecs.GetComponentForEntity<Velocity>(entity);
	//std::cout << "Velocity: " << vel->vx << ", " << vel->vy << ", " << vel->vz << " | ";
	//Position* pos = _data->ecs.GetComponentForEntity<Position>(entity);
	//std::cout << "Position: " << pos->x << ", " << pos->y << ", " << pos->z << " | ";
	//Orientation * orient = _data->ecs.GetComponentForEntity<Orientation>(entity);
	//std::cout << "Orientation: " << orient->yaw << ", " << orient->pitch << std::endl;
}

void StartState::Draw(float dt)
{
	TransformComponent* transform = _data->ecs.GetComponentForEntity<TransformComponent>(player);

	_data->camera.position = transform->position;
	_data->camera.rotation = transform->rotation;

}

void StartState::Pause()
{
}

void StartState::Resume()
{
}
