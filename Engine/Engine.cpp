#include "pch.h"
#include "Engine.h"
#include "Filter.h"

using namespace BlahEngine;

Engine::Engine() :
	_isInited(false),
	_window(new Window),
	_renderModule(new RenderModule),
	_uiModule(new UiModule),
	_inputModule(new InputModule),
	_timeModule(new TimeModule),
	_statsModule(new StatsModule),
	_gameplayModule(new GameplayModule),
	_physicsModule(new PhysicsModule),
	_transformsFilter(nullptr) {}

Engine::~Engine()
{
	delete _uiModule;
	delete _renderModule;
	delete _physicsModule;
	delete _gameplayModule;
	delete _window;
	delete _inputModule;
	delete _statsModule;
	delete _timeModule;
}

bool Engine::Init()
{
	_timeModule->Init();
	_inputModule->Init();

	WindowDesc windowDesc;
	windowDesc.Width = 640; //854
	windowDesc.Height = 480;
	_window->Create(windowDesc);
	_window->AttachInput(_inputModule);
	
	_gameplayModule->Init(this);

	_statsModule->Init(_timeModule, _gameplayModule->GetEcs());

	_renderModule->Init(_window->GetHWND(), windowDesc.Width, windowDesc.Height, _timeModule, _gameplayModule->GetEcs());
	_uiModule->Init(_renderModule->GetSwapChain(), _gameplayModule->GetEcs());
	_physicsModule->Init(_gameplayModule->GetEcs());

	_gameplayModule->InitSystems();

	_transformsFilter = _gameplayModule->GetEcs()->GetFilter(FilterMask().Inc<TransformComp>());

	_isInited = true;
	return true;
}


void Engine::Run()
{
	if (!_isInited)
		throw std::exception("engine is not initialized");

	while (true)
	{
		_timeModule->Run();

		_window->Run();

		if (!_window->IsActive())
			continue;

		if (_window->IsExit())
			break;

		if (_window->IsResize()) { }

		_inputModule->Run();

		_statsModule->BeginGameplay();
		_gameplayModule->RunSystems();
		_statsModule->EndGameplay();

		_inputModule->ClearEvents();

		_statsModule->BeginRender();
		_renderModule->BeginDrawFrame();
		_renderModule->DrawFrame();
		_uiModule->DrawFrame();
		_renderModule->EndDrawFrame();
		_statsModule->EndRender();

		_statsModule->RunDisplay();

		for (auto entity : *_transformsFilter)
			_gameplayModule->GetEcs()->GetComp<TransformComp>(entity).ResetIsChanged();
	}
}


IRenderModule* Engine::Render()
{
	return _renderModule;
}

IPhysicsModule* Engine::Physics()
{
	return _physicsModule;
}

IGameplayModule* Engine::Gameplay()
{
	return _gameplayModule;
}

const InputModule* Engine::Input()
{
	return _inputModule;
}

const TimeModule* Engine::Time()
{
	return _timeModule;
}

IStatsModule* Engine::Stats()
{
	return _statsModule;
}
