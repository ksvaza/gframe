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
	InputManager Input;
	Renderer Render;

	Shader testShader;
	Mesh testMesh;
};
