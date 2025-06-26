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
	entity = _data->ecs.CreateEntity();
	_data->ecs.RegisterComponent<Velocity>();
	_data->ecs.RegisterComponent<InputTag>();
	_data->ecs.RegisterComponent<PlayerTag>();
	_data->ecs.RegisterComponent<TransformComponent>();
	_data->ecs.RegisterComponent<RenderTag>();
	_data->ecs.RegisterComponent<MeshComponent>();

	_data->ecs.AddComponent<Velocity>(entity, Velocity(0.0f, 0.0f, 0.0f ));
	_data->ecs.AddComponent<InputTag>(entity, InputTag());
	_data->ecs.AddComponent<PlayerTag>(entity, PlayerTag());
	_data->ecs.AddComponent<TransformComponent>(entity, TransformComponent());
	_data->ecs.AddComponent<RenderTag>(entity, RenderTag("testShader"));
	
	std::cout << "shader key: " << _data->ecs.GetComponentForEntity<RenderTag>(entity)->ShaderKey << '\n';

	_data->ecs.RegisterSystem(MovementSystem);
	_data->ecs.RegisterSystem(UpdateMovement);
	_data->ecs.RegisterSystem(RenderSystem);
	yaw = -90.0f;
	pitch = 0;
	CameraPos = glm::vec3(0.0f, 0.0f, 30.0f);

	_data->AssetManager.LoadAllMeshesFromFolder("../StartState");


	//_data->ecs.AddComponent<MeshComponent>(entity, MeshComponent(_data->AssetManager.GetMeshComponent("cube")));



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
	float sensitivity = 0.1f;
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
	}
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
	TransformComponent* transform = _data->ecs.GetComponentForEntity<TransformComponent>(entity);

	_data->camera.position = transform->position;
	_data->camera.rotation = transform->rotation;

	/*for (int i = 0; i < 4; i++)
	{
		std::cout << projectionMatrix[i][0] << ", " 
			<< viewMatrix[i][1] << ", "
			<< viewMatrix[i][2] << ", "
			<< viewMatrix[i][3] << " | "
			<< _data->camera.GetViewMatrix()[i][0] << ", "
			<< _data->camera.GetViewMatrix()[i][1] << ", "
			<< _data->camera.GetViewMatrix()[i][2] << ", "
			<< _data->camera.GetViewMatrix()[i][3] << '\n';
	}*/

	Render.Clear(glm::vec4(0.8, 0.0, 0.0, 1.0));
	Render.DrawMesh(testMesh2, testShader, _data->camera.GetViewMatrix(), _data->camera.GetProjectionMatrix());
}

void StartState::Pause()
{
}

void StartState::Resume()
{
}
