#include <iostream>
#include "BlahEngine.h"
#include "CameraSystem.h"
#include "PlanetsSystem.h"

using namespace BlahEngine;


int main()
{
    Engine engine;
    engine.Init();
    
    engine.Systems().AddSystem(new CameraSystem(5.0f));
    engine.Systems().AddSystem(new PlanetsSystem);

    engine.Run();
}