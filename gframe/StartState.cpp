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

	_data->AssetManager.LoadAllMeshesFromFolder("../StartState");

	player = _data->ecs.CreateEntity();
	cube = _data->ecs.CreateEntity();
	house = _data->ecs.CreateEntity();

	_data->ecs.RegisterComponent<Velocity>();
	_data->ecs.RegisterComponent<InputTag>();
	_data->ecs.RegisterComponent<PlayerTag>();
	_data->ecs.RegisterComponent<TransformComponent>();
	_data->ecs.RegisterComponent<RenderTag>();
	_data->ecs.RegisterComponent<MeshComponent>();

	_data->ecs.AddComponent<Velocity>(player);
	_data->ecs.AddComponent<InputTag>(player);
	_data->ecs.AddComponent<PlayerTag>(player);
	_data->ecs.AddComponent<TransformComponent>(player);


	_data->ecs.AddComponentData<MeshComponent>(house, MeshComponent(_data->AssetManager.GetMeshComponent("house")));
	_data->ecs.AddComponentData<TransformComponent>(house, TransformComponent(glm::vec3(0, 0, 0)));
	_data->ecs.AddComponentData<RenderTag>(house, RenderTag("testShader"));
		
	_data->ecs.AddComponentData<MeshComponent>(cube, MeshComponent(_data->AssetManager.GetMeshComponent("cube")));
	_data->ecs.AddComponentData<TransformComponent>(cube, TransformComponent(glm::vec3(0, 0, 0)));
	_data->ecs.AddComponentData<RenderTag>(cube, RenderTag("testShader"));
	


	_data->ecs.RegisterInputSystem(MovementSystem);
	_data->ecs.RegisterUpdateSystem(UpdateMovement);
	_data->ecs.RegisterRenderSystem(RenderSystem);


	/*Pixel3* pixels = _data->AssetManager.GetTexture("check").data.ch3;

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
	testMesh2.transform.scale = glm::vec3(1.0f);*/

	_data->AssetManager.LoadShader("testShader", "vertex.glsl", "fragment.glsl");

	printf("Start State initialised!\n");
}

void StartState::HandleInput(float dt)
{
	_data->ecs.UpdateInputSystems(_data.get(), dt);
}

void StartState::Update(float dt)
{
	_data->ecs.UpdateSystems(_data.get(), dt);
}

void StartState::Draw(float dt)
{
	_data->Render.Clear(glm::vec4(0.8, 0.0, 0.0, 1.0));
	_data->ecs.UpdateRenderSystems(_data.get(), dt);

	
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
