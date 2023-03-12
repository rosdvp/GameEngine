#include "pch.h"
#include "GameplayModule.h"

#include "Engine.h"

using namespace BlahEngine;

GameplayModule::GameplayModule() :
	_ecsCore(nullptr),
	_isInited(false) {}

GameplayModule::~GameplayModule()
{
	for (auto system : _allSystems)
		delete system;
	delete _ecsCore;

	Logger::Debug("gameplay module", "destroyed");
}

void GameplayModule::Init(Engine* engine)
{
	_ecsCore = new EcsCore();

	for (auto system : _allSystems)
		system->AttachEngine(engine, _ecsCore);

	_isInited = true;
	Logger::Debug("gameplay module", "initialized");
}

void GameplayModule::InitSystems()
{
	for (auto system : _initSystems)
		system->Init();
	Logger::Debug("gameplay module", "system initialized");
}

void GameplayModule::RunSystems()
{
	for (auto system : _runSystems)
		system->Run();
}

void GameplayModule::AddSystem(ISystem* system)
{
	if (_isInited)
		throw std::exception("cannot add system after initializing");

	_allSystems.push_back(system);

	auto initSystem = dynamic_cast<IInitSystem*>(system);
	if (initSystem)
		_initSystems.push_back(initSystem);

	auto runSystem = dynamic_cast<IRunSystem*>(system);
	if (runSystem)
		_runSystems.push_back(runSystem);
}

EcsCore* GameplayModule::GetEcs()
{
	return _ecsCore;
}
