#pragma once

namespace BlahEngine
{
class TimeModule;
struct Vector3;

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

	void Init(entt::registry* ecs, const TimeModule* timeModule);

	void Run();

	void BeginGameplay();
	void EndGameplay();
	void BeginPhysics();
	void EndPhysics();
	void BeginRender();
	void EndRender();

	void ShowDisplay(const Vector3& pos) override;
	void HideDisplay() override;

private:
	entt::registry* _ecs;
	const TimeModule* _timeModule;

	std::chrono::time_point<std::chrono::steady_clock> _gameplayStartTime;
	float _gameplayMS;
	std::chrono::time_point<std::chrono::steady_clock> _physicsStartTime;
	float _physicsMS;
	std::chrono::time_point<std::chrono::steady_clock> _renderStartTime;
	float _renderMS;
	
	entt::entity _entDisplay;
};
}
