#pragma once
#include "EcsCore.h"
#include "TimeModule.h"
#include "Vector3.h"

namespace BlahEngine
{
class IStatsModule
{
public:
	virtual void ShowDisplay(const Vector3& pos) = 0;
	virtual void HideDisplay() = 0;
};

class StatsModule : public virtual IStatsModule
{
public:
	StatsModule();
	~StatsModule();

	void Init(TimeModule* timeModule, EcsCore* ecs);

	void RunDisplay();

	void BeginGameplay();
	void EndGameplay();
	void BeginRender();
	void EndRender();

	void ShowDisplay(const Vector3& pos) override;
	void HideDisplay() override;

private:
	TimeModule* _timeModule;
	EcsCore* _ecs;

	std::chrono::time_point<std::chrono::steady_clock> _gameplayStartTime;
	float _gameplayMS;
	std::chrono::time_point<std::chrono::steady_clock> _renderStartTime;
	float _renderMS;

	bool _isDisplaying;
	Entity _eDisplay;
};
}
