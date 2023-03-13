#pragma once
//#include "PhysicsModule.h"
#include "GameplayModule.h"
#include "InputModule.h"
#include "Logger.h"
#include "PhysicsModule.h"
#include "RenderModule.h"
#include "StatsModule.h"
#include "TimeModule.h"
#include "UiModule.h"
#include "Window.h"

namespace BlahEngine
{
class Engine
{
public:
	Engine();
	~Engine();

	bool Init();
	void Run();

	IGameplayModule* Gameplay();
	IRenderModule* Render();
	IPhysicsModule* Physics();
	const InputModule* Input();
	const TimeModule* Time();
	IStatsModule* Stats();

private:
	bool _isInited;

	Logger _logger;

	Window* _window;
	RenderModule* _renderModule;
	UiModule* _uiModule;
	InputModule* _inputModule;
	TimeModule* _timeModule;
	StatsModule* _statsModule;
	GameplayModule* _gameplayModule;
	PhysicsModule* _physicsModule;

	Filter* _transformsFilter;
};
}
