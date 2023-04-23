#include "pch.h"
#include "Engine.h"

using namespace BlahEngine;

Engine::Engine() :
	_isInited(false) {}

Engine::~Engine()
{
}

bool Engine::Init()
{
	_timeModule.Init();
	_inputModule.Init();

	WindowDesc windowDesc;
	windowDesc.Width = 640; //854
	windowDesc.Height = 480;
	_window.Create(windowDesc);
	_window.AttachInput(&_inputModule);
	
	_systemsModule.Init(this, &_ecs);

	_statsModule.Init(&_ecs, &_timeModule);

	_transformsModule.Init(&_ecs);
	_physicsModule.Init(&_ecs);

	_renderModule.Init(&_ecs, &_timeModule, _window.GetHWND(), windowDesc.Width, windowDesc.Height);
	_uiModule.Init(&_ecs, _renderModule.GetSwapChain());
	
	_isInited = true;
	return true;
}


void Engine::Run()
{
	if (!_isInited)
		throw std::exception("engine is not initialized");

	while (true)
	{
		_timeModule.Run();

		_window.Run();

		if (!_window.IsActive())
			continue;

		if (_window.IsExit())
			break;

		if (_window.IsResize()) { }

		_inputModule.Run();

		_statsModule.BeginGameplay();
		_systemsModule.Run();
		_statsModule.EndGameplay();

		_inputModule.Clear();

		_transformsModule.AdjustGlobalPositions();

		_statsModule.BeginPhysics();
		_physicsModule.RunCollisions();
		_statsModule.EndPhysics();

		_statsModule.BeginRender();
		_renderModule.BeginFrame();
		_renderModule.DrawFrame();
		_uiModule.DrawFrame();
		_renderModule.EndFrame();
		_statsModule.EndRender();

		_transformsModule.ResetTransformsIsChanged();

		_statsModule.Run();
	}
}

entt::registry& Engine::Ecs()
{
	return _ecs;
}


IRenderModule& Engine::Render()
{
	return _renderModule;
}

ISystemsModule& Engine::Systems()
{
	return _systemsModule;
}

const InputModule& Engine::Input()
{
	return _inputModule;
}

const TimeModule& Engine::Time()
{
	return _timeModule;
}

IStatsModule& Engine::Stats()
{
	return _statsModule;
}
