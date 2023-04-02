#pragma once
#include "ISystem.h"


namespace BlahEngine
{
class Engine;

class ISystemsModule
{
public:
	virtual ~ISystemsModule() = default;

	virtual void AddSystem(ISystem* system) = 0;
};

class SystemsModule : virtual public ISystemsModule
{
public:
	SystemsModule();
	~SystemsModule() override;

	void Init(Engine* engine, entt::registry* ecs);
	
	void Run();

	void AddSystem(ISystem* system) override;

private:
	Engine* _engine;
	entt::registry* _ecs;

	bool _isSystemsInited;

	std::vector<std::unique_ptr<ISystem>> _allSystems;
	std::vector<IInitSystem*> _initSystems;
	std::vector<IRunSystem*> _runSystems;
};
}