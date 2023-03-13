#include "pch.h"
#include "StatsModule.h"

#include "EcsCore.h"
#include "Logger.h"
#include "TimeModule.h"
#include "TransformComp.h"
#include "UiTextComp.h"

using namespace BlahEngine;

StatsModule::StatsModule():
	_timeModule(nullptr),
	_ecs(nullptr),
	_isDisplaying(false),
	_eDisplay() {}

StatsModule::~StatsModule()
{
	Logger::Debug("stats", "destroyed");
}

void StatsModule::Init(TimeModule* timeModule, EcsCore* ecs)
{
	_timeModule = timeModule;
	_ecs = ecs;
	Logger::Debug("stats", "initialized");
}

void StatsModule::RunDisplay()
{
	if (!_isDisplaying)
		return;
	
	int fps = static_cast<int>(1.0f / _timeModule->GetFrameDeltaSecs());

	auto& text = _ecs->GetComp<UiTextComp>(_eDisplay);
	text.Text = L"fps: " + std::to_wstring(fps) +
		L"\nms: " + std::to_wstring(_timeModule->GetFrameDeltaSecs() * 1000) +
		L"\ngame: " + std::to_wstring(_gameplayMS * 1000) +
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
	if (_isDisplaying)
		return;
	
	_isDisplaying = true;
	_eDisplay = _ecs->CreateEntity();

	auto& tf = _ecs->AddComp<TransformComp>(_eDisplay);
	tf.Pos = pos;
	tf.Scale = { 100, 1, 1 };

	auto& text = _ecs->AddComp<UiTextComp>(_eDisplay);
	text.Font = L"Arial";
	text.FontSize = 10;
}

void StatsModule::HideDisplay()
{
	if (!_isDisplaying)
		return;;

	_isDisplaying = false;
	_ecs->DestroyEntity(_eDisplay);
}
