#pragma once
#include "EcsCore.h"
#include "TimeModule.h"

namespace BlahEngine
{
class StatsModule
{
public:
	StatsModule();
	~StatsModule();

	void Init(TimeModule* timeModule, EcsCore* ecs);

	void Run();

	void ShowDisplay();
	void HideDisplay();

private:
	TimeModule* _timeModule;
	EcsCore* _ecs;

	bool _isDisplaying;
	Entity _eDisplay;
};
}
