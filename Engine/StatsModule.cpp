#include "pch.h"
#include "StatsModule.h"

#include "Logger.h"
#include "TimeModule.h"
#include "TransformComp.h"
#include "UiTextComp.h"

using namespace BlahEngine;

StatsModule::StatsModule():
	_ecs(nullptr),
	_timeModule(nullptr),
	_gameplayMS(0),
	_physicsMS(0),
	_renderMS(0),
	_entDisplay(entt::null) {}

StatsModule::~StatsModule()
{
	Logger::Debug("stats", "destroyed");
}

void StatsModule::Init(entt::registry* ecs, const TimeModule* timeModule)
{
	_ecs = ecs;
	_timeModule = timeModule;
	Logger::Debug("stats", "initialized");
}

void StatsModule::Run()
{
	if (!_ecs->valid(_entDisplay))
		return;

	auto& txt = _ecs->get<UiTextComp>(_entDisplay);

	int fps = static_cast<int>(1.0f / _timeModule->GetFrameDeltaSecs());

	txt.Text = L"fps: " + std::to_wstring(fps) +
		L"\nms: " + std::to_wstring(_timeModule->GetFrameDeltaSecs() * 1000) +
		L"\ngame: " + std::to_wstring(_gameplayMS * 1000) +
		L"\nphysics: " + std::to_wstring(_physicsMS * 1000) +
		L"\nrender: " + std::to_wstring(_renderMS * 1000);
}

void StatsModule::BeginGameplay()
{
	_gameplayStartTime = std::chrono::steady_clock::now();
}
void StatsModule::EndGameplay()
{
	auto currTime = std::chrono::steady_clock::now();
	_gameplayMS = std::chrono::duration_cast<std::chrono::microseconds>(currTime - _gameplayStartTime).count()
		/ 1000000.0f;
}

void StatsModule::BeginPhysics()
{
	_physicsStartTime = std::chrono::steady_clock::now();
}
void StatsModule::EndPhysics()
{
	auto currTime = std::chrono::steady_clock::now();
	_physicsMS = std::chrono::duration_cast<std::chrono::microseconds>(currTime - _physicsStartTime).count()
		/ 1000000.0f;
}

void StatsModule::BeginRender()
{
	_renderStartTime = std::chrono::steady_clock::now();
}
void StatsModule::EndRender()
{
	auto currTime = std::chrono::steady_clock::now();
	_renderMS = std::chrono::duration_cast<std::chrono::microseconds>(currTime - _renderStartTime).count()
		/ 1000000.0f;
}

void StatsModule::ShowDisplay(const Vector3& pos)
{
	if (_ecs->valid(_entDisplay))
		return;

	_entDisplay = _ecs->create();

	auto& tf = _ecs->emplace<TransformComp>(_entDisplay);
	tf.Pos = pos;
	tf.Scale = { 100, 1, 1 };

	auto& text = _ecs->emplace<UiTextComp>(_entDisplay);
	text.Font = L"Arial";
	text.FontSize = 10;
}

void StatsModule::HideDisplay()
{
	if (!_ecs->valid(_entDisplay))
		return;

	_ecs->destroy(_entDisplay);
	_entDisplay = entt::null;
}
