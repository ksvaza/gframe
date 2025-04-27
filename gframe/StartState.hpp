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
	void HandleInput() override;
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
	glm::vec3 CameraPos;
	glm::vec3 CameraFront;

	float yaw;
	float pitch;
};
