#pragma once
#include "ISystem.h"
#include "EcsCore.h"


namespace BlahEngine
{
class Engine;

class IGameplayModule
{
public:
	virtual ~IGameplayModule() = default;

	virtual EcsCore* GetEcs() = 0;

	virtual void AddSystem(ISystem* system) = 0;
};

class GameplayModule : virtual public IGameplayModule
{
public:
	GameplayModule();
	~GameplayModule() override;

	void Init(Engine* engine);

	void InitSystems();
	void RunSystems();

	EcsCore* GetEcs() override;

	void AddSystem(ISystem* system) override;

private:
	EcsCore* _ecsCore;

	bool _isInited;

	std::vector<ISystem*> _allSystems;
	std::vector<IInitSystem*> _initSystems;
	std::vector<IRunSystem*> _runSystems;
};
}