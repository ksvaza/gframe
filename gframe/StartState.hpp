#include "State.hpp"
#include <stdio.h>
#include "InputManager.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "mesh.hpp"

class StartState : public State
{
public:
	StartState(GframeDataRef data) : _data(data) {};
	~StartState() {};

	void Init() override;
	void HandleInput(float dt) override;
	void Update(float dt) override;
	void Draw(float dt) override;


	void Pause() override;
	void Resume() override;

private:
	GframeDataRef _data;
	InputManager Input = _data->Input;
	Renderer Render = _data->Render;

	Shader testShader;
	Mesh testMesh;
	Mesh testMesh2;

	float yaw;
	float pitch;

	EntityID player;
	EntityID cube;
	EntityID house;
};
