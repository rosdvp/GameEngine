#include "BallSystem.h"
#include "BlahEngine.h"
#include "CameraSystem.h"
#include "MatrixRenderShader.h"
#include "PlayerSystem.h"
#include "ScoreSystem.h"

using namespace BlahEngine;

int main()
{
	Engine engine;

	engine.Render()->AddRenderShader(new MatrixRenderShader);

	engine.Gameplay()->AddSystem(new CameraSystem);
	engine.Gameplay()->AddSystem(new PlayerSystem(
		0, 10.0f, -10, 7, { 0.3f, 1.5f, 1.0f } ));
	engine.Gameplay()->AddSystem(new PlayerSystem(
		1, 10.0f, 10, 7, { 0.3f, 1.5f, 1.0f }));
	engine.Gameplay()->AddSystem(new BallSystem(
		12, 8,
		{ 0.3f, 0.3f, 0.3f },
		5.0f, 1.05f));
	engine.Gameplay()->AddSystem(new ScoreSystem);

	engine.Init();

	engine.Run();

	return 0;
}