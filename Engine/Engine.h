#pragma once

#include "entt.hpp"

#include "SystemsModule.h"
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

	entt::registry& Ecs();
	ISystemsModule& Systems();
	IRenderModule& Render();
	const InputModule& Input();
	const TimeModule& Time();
	IStatsModule& Stats();

private:
	bool _isInited;

	entt::registry _ecs;

	Logger _logger;

	Window _window;
	RenderModule _renderModule;
	UiModule _uiModule;
	InputModule _inputModule;
	TimeModule _timeModule;
	StatsModule _statsModule;
	SystemsModule _systemsModule;
	PhysicsModule _physicsModule;
};
}
