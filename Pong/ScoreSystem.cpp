#include "ScoreSystem.h"

#include "ScoreIncreaseCmd.h"
#include "UiTextComp.h"

using namespace BlahEngine;

void ScoreSystem::Init()
{
	_entity = _ecs->CreateEntity();
	auto& tf = _ecs->AddComp<TransformComp>(_entity);
	tf.Pos = { 0, -200, 0 };
	tf.Scale = { 200, 1, 1 };
	auto& textComp = _ecs->AddComp<UiTextComp>(_entity);
	textComp.Font = L"Arial";
	textComp.FontSize = 50;
	textComp.IsBold = true;
	textComp.Text = L"0:0";

	_scoreIncreaseCmdFilter = _ecs->GetFilter(FilterMask().Inc<ScoreIncreaseCmd>());
}

void ScoreSystem::Run()
{
	for (auto entity : *_scoreIncreaseCmdFilter)
	{
		auto& cmd = _ecs->GetComp<ScoreIncreaseCmd>(entity);
		if (cmd.PlayerId == 0)
			_score0 += 1;
		else
			_score1 += 1;
		_ecs->RemoveComp<ScoreIncreaseCmd>(entity);

		auto& textComp = _ecs->GetComp<UiTextComp>(_entity);
		textComp.Text = std::to_wstring(_score0) + L":" + std::to_wstring(_score1);
	}
}