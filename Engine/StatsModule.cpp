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
	ShowDisplay();
	Logger::Debug("stats", "initialized");
}

void StatsModule::Run()
{
	if (!_isDisplaying)
		return;

	float ms = _timeModule->GetFrameDeltaSecs() * 1000;
	int fps = static_cast<int>(1.0f / ms);

	auto& text = _ecs->GetComp<UiTextComp>(_eDisplay);
	text.Text = L"fps: " + std::to_wstring(fps) + L", ms: " + std::to_wstring(ms);
}

void StatsModule::ShowDisplay()
{
	if (_isDisplaying)
		return;
	
	_isDisplaying = true;
	_eDisplay = _ecs->CreateEntity();

	auto& tf = _ecs->AddComp<TransformComp>(_eDisplay);
	tf.Pos = { 0, 0, 0 };
	tf.Scale = { 100, 1, 1 };

	auto& text = _ecs->AddComp<UiTextComp>(_eDisplay);
	text.Font = L"Arial";
	text.FontSize = 20;
}

void StatsModule::HideDisplay()
{
	if (!_isDisplaying)
		return;;

	_isDisplaying = false;
	_ecs->DestroyEntity(_eDisplay);
}
