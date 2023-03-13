#include <iostream>
#include "BlahEngine.h"
#include "CameraSystem.h"
#include "PlanetsSystem.h"

using namespace BlahEngine;


int main()
{
    Engine engine;
    
    engine.Gameplay()->AddSystem(new CameraSystem(5.0f));
    engine.Gameplay()->AddSystem(new PlanetsSystem);

    engine.Init();
    engine.Run();
}