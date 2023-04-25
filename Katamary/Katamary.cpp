#include "BlahEngine.h"
#include "LevelSystem.h"
#include "PlayerCameraSystem.h"
#include "PlayerMoveSystem.h"
#include "CollectSystem.h"
#include "SunLightSystem.h"

using namespace BlahEngine;


int main()
{
	Engine engine;
	engine.Init(1280, 720);
	
	engine.Systems().AddSystem(new LevelSystem);
	engine.Systems().AddSystem(new PlayerMoveSystem);
	engine.Systems().AddSystem(new PlayerCameraSystem);
	engine.Systems().AddSystem(new CollectSystem);
	engine.Systems().AddSystem(new SunLightSystem);

	engine.Run();

	return 0;
}