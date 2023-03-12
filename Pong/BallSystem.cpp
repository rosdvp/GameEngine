#include "BallSystem.h"

#include "ScoreIncreaseCmd.h"
#include "SquareRenderBuilder.h"

using namespace BlahEngine;

BallSystem::BallSystem(
	float borderPosX, float borderPosY, 
	Vector3 ballScale, 
	float moveBaseSpeed, float moveSpeedIncK) :
	_borderPosX(borderPosX),
	_borderPosY(borderPosY),
	_ballScale(ballScale),
	_moveBaseSpeed(moveBaseSpeed),
	_moveSpeedIncK(moveSpeedIncK),
	_moveSpeed(moveBaseSpeed)
{
}

void BallSystem::Init()
{
	_entity = _ecs->CreateEntity();

	_ecs->AddComp<TransformComp>(_entity).Scale = _ballScale;
	_ecs->AddComp<BoxCollisionComp>(_entity);

	auto& render = _ecs->AddComp<RenderComp>(_entity);
	SquareRenderBuilder::Build(render);

	_dir = { -1, -1, 0 };
}

void BallSystem::Run()
{
	while (!TryMove()) { }

	auto& tf = _ecs->GetComp<TransformComp>(_entity);
	bool isPlayer0Goal = tf.Pos.X > _borderPosX;
	bool isPlayer1Goal = tf.Pos.X < -_borderPosX;

	if (isPlayer0Goal || isPlayer1Goal)
	{
		auto entity = _ecs->CreateEntity();
		_ecs->AddComp<ScoreIncreaseCmd>(entity).PlayerId = isPlayer0Goal ? 0 : 1;

		_dir = -_dir;
		_moveSpeed = _moveBaseSpeed;
		tf.Pos = { 0, 0, 0 };
	}
}

bool BallSystem::TryMove()
{
	auto& tf = _ecs->GetComp<TransformComp>(_entity);

	Vector3 oldPos{ tf.Pos };
	Vector3 delta{ _dir * _moveSpeed * _engine->Time()->GetFrameDeltaSecs() };
	tf.Pos += delta;

	PhysicsBlockData data;
	if (_engine->Physics()->IsBlockAny(_entity, data))
	{
		_dir = data.DirToEnt;
		_dir.X = _dir.X < 0 ? -1 : 1;
		_dir.Y = _dir.Y < 0 ? -1 : 1;
		_dir.Z = 0;

		tf.Pos = oldPos;

		_moveSpeed *= _moveSpeedIncK;

		return false;
	}

	if (tf.Pos.Y < -_borderPosY || tf.Pos.Y > _borderPosY)
	{
		_dir.Y = -_dir.Y;
		tf.Pos = oldPos;
		return false;
	}

	return true;
}
