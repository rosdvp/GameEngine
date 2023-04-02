#include "pch.h"
#include "SystemsModule.h"

#include "BlahEngine.h"
#include "Engine.h"

using namespace BlahEngine;

SystemsModule::SystemsModule() :
	_isSystemsInited(false) {}

SystemsModule::~SystemsModule()
{
	Logger::Debug("gameplay module", "destroyed");
}

void SystemsModule::Init(Engine* engine, entt::registry* ecs)
{
	_engine = engine;
	_ecs = ecs;
	Logger::Debug("gameplay module", "initialized");
}

void SystemsModule::Run()
{
	if (!_isSystemsInited)
	{
		for (auto system : _initSystems)
			system->Init();

		_isSystemsInited = true;
		Logger::Debug("gameplay module", "system initialized");
	}

	for (auto system : _runSystems)
		system->Run();
}

void SystemsModule::AddSystem(ISystem* system)
{
	_allSystems.push_back(std::unique_ptr<ISystem>(system));
	auto& sys = _allSystems.back();

	sys->AttachEngine(_engine, _ecs);

	auto initSystem = dynamic_cast<IInitSystem*>(sys.get());
	if (initSystem)
		_initSystems.push_back(initSystem);

	auto runSystem = dynamic_cast<IRunSystem*>(sys.get());
	if (runSystem)
		_runSystems.push_back(runSystem);
}
