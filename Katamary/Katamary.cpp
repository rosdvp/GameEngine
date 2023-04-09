#include "BlahEngine.h"
#include "LevelSystem.h"
#include "PlayerCameraSystem.h"
#include "PlayerSystem.h"

using namespace BlahEngine;

int main()
{
	Engine engine;
	engine.Init();
	
	engine.Systems().AddSystem(new LevelSystem);
	engine.Systems().AddSystem(new PlayerSystem);
	engine.Systems().AddSystem(new PlayerCameraSystem);

	engine.Run();

	return 0;
}