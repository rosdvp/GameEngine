#include "ScoreSystem.h"

#include "ScoreIncreaseCmd.h"
#include "UiTextComp.h"

using namespace BlahEngine;

void ScoreSystem::Init()
{
	_ent = _ecs->create();
	auto& tf = _ecs->emplace<TransformComp>(_ent);
	tf.Pos = { 0, -200, 0 };
	tf.Scale = { 200, 1, 1 };
	auto& txt = _ecs->emplace<UiTextComp>(_ent);
	txt.Font = L"Arial";
	txt.FontSize = 50;
	txt.IsBold = true;
	txt.Text = L"0:0";
}

void ScoreSystem::Run()
{
	auto view = _ecs->view<ScoreIncreaseCmd>();
	for (auto ent : view)
	{
		auto& cmd = view.get<ScoreIncreaseCmd>(ent);
		if (cmd.PlayerId == 0)
			_score0++;
		else
			_score1++;

		auto& txt = _ecs->get<UiTextComp>(_ent);
		txt.Text = std::to_wstring(_score0) + L":" + std::to_wstring(_score1);

		_ecs->destroy(ent);
	}
}