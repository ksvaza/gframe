#include "gframe.hpp"
#include "StartState.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include "texture.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "Position.hpp"
#include "Velocity.hpp"
#include "Orientation.hpp"
#include "InputTag.hpp"
#include "MovementSystem.hpp"
#include "Movement.hpp"



void StartState::Init()
{
	entity = _data->ecs.CreateEntity();
	_data->ecs.RegisterComponent<Position>();
	_data->ecs.RegisterComponent<Velocity>();
	_data->ecs.RegisterComponent<Orientation>();
	_data->ecs.RegisterComponent<InputTag>();
	_data->ecs.RegisterComponent<PlayerTag>();

	_data->ecs.AddComponent<Position>(entity, Position{ 0.0f, 0.0f, 0.0f });
	_data->ecs.AddComponent<Velocity>(entity, Velocity{ 0.0f, 0.0f, 0.0f });
	_data->ecs.AddComponent<Orientation>(entity, Orientation{ 0.0f, 0.0f });
	_data->ecs.AddComponent<InputTag>(entity, InputTag{});
	_data->ecs.AddComponent<PlayerTag>(entity, PlayerTag{});

	_data->ecs.RegisterSystem(MovementSystem);
	_data->ecs.RegisterSystem(UpdateMovement);

	/*_data->ecs.RegisterSystem([this](ECS::ecs& ecs, float dt) {
		_data->window.height;


		});*/

	yaw = -90.0f;
	pitch = 0;
	CameraPos = glm::vec3(0.0f, 0.0f, 30.0f);

	_data->AssetManager.LoadAllMeshesFromFolder("../StartState");
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
	Velocity* vel = _data->ecs.GetComponentForEntity<Velocity>(entity);
	//std::cout << "Velocity: " << vel->vx << ", " << vel->vy << ", " << vel->vz << " | ";
	Position* pos = _data->ecs.GetComponentForEntity<Position>(entity);
	//std::cout << "Position: " << pos->x << ", " << pos->y << ", " << pos->z << " | ";
	Orientation * orient = _data->ecs.GetComponentForEntity<Orientation>(entity);
	//std::cout << "Orientation: " << orient->yaw << ", " << orient->pitch << std::endl;
}

void StartState::Draw(float dt)
{
	Position* pos = _data->ecs.GetComponentForEntity<Position>(entity);
	Orientation* orient = _data->ecs.GetComponentForEntity<Orientation>(entity);
	
	CameraFront = glm::normalize(glm::vec3(cos(glm::radians(orient->yaw)) * cos(glm::radians(orient->pitch)),
		sin(glm::radians(orient->pitch)),
		sin(glm::radians(orient->yaw)) * cos(glm::radians(orient->pitch))));

	glm::mat4 viewMatrix = glm::lookAt(
		glm::vec3(pos->x, pos->y, pos->z), //camera pos
		CameraFront + glm::vec3(pos->x, pos->y, pos->z), //look at pos !!not look at vector!!
		glm::vec3(0.0f, 1.0f, 0.0f)  //Up Direction
	);

	float aspectRatio = (float)_data->window.width / (float)_data->window.height;
	glm::mat4 projectionMatrix = glm::perspective(
		glm::radians(80.0f),
		aspectRatio,
		0.1f,  //tuvais grieziens
		1000.0f //talais grieziens
	);


	Render.Clear(glm::vec4(0.8, 0.0, 0.0, 1.0));
	Render.DrawMesh(testMesh2, testShader, viewMatrix, projectionMatrix);
}

void StartState::Pause()
{
}

void StartState::Resume()
{
}
