#include "BallSystem.h"
#include "BlahEngine.h"
#include "CameraSystem.h"
#include "PlayerSystem.h"
#include "ScoreSystem.h"

using namespace BlahEngine;

int main()
{
	Engine engine;
	engine.Init();

	engine.Systems().AddSystem(new CameraSystem);
	engine.Systems().AddSystem(new PlayerSystem(
		0, 10.0f, -10, 7, { 0.3f, 1.5f, 1.0f }));
	engine.Systems().AddSystem(new PlayerSystem(
		1, 10.0f, 10, 7, { 0.3f, 1.5f, 1.0f }));
	engine.Systems().AddSystem(new BallSystem(
		12, 8,
		{ 0.3f, 0.3f, 0.3f },
		5.0f, 1.05f));
	engine.Systems().AddSystem(new ScoreSystem);

	engine.Stats().ShowDisplay({0, -100, 0});

	engine.Run();

	return 0;
}