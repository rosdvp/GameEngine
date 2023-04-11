#include "PlayerSystem.h"

#include "GeometryRenderBuilder.h"
#include "PlayerComp.h"
#include "RenderComp.h"

using namespace BlahEngine;

void PlayerSystem::Init()
{
	_entPlayer = _ecs->create();

	_ecs->emplace<PlayerComp>(_entPlayer);

	auto& tf = _ecs->emplace<TransformComp>(_entPlayer);
	tf.Pos = { 0, 1.5f, 0 };
	tf.Scale = { 1, 1, 1 };

	auto& render = _ecs->emplace<RenderComp>(_entPlayer);
	render.ShaderId = RenderModule::EShaderId::SimpleLit;
	render.Mat = {0.5f, 0.1f, 0.5f, 30};
	//render.Mat = { 0.2775f, 0.23125f, 0.773911f, 89.6 };

	GeometryRenderBuilder::BuildSphere(render, 10, Color::Pink());
	for (int i = 0; i < render.VerticesCount; i += 5)
		render.Vertices[i].Color = Color::Yellow();
}

void PlayerSystem::Run()
{
	if (_entCamera == entt::null)
	{
		auto view = _ecs->view<const RenderCameraComp>();
		if (!view.empty())
			_entCamera = view.front();
		return;
	}

	float deltaTime = _engine->Time().GetFrameDeltaSecs();

	auto& playerTf = _ecs->get<TransformComp>(_entPlayer);
	auto& cameraTf = _ecs->get<TransformComp>(_entCamera);

	auto forward = cameraTf.GetForward();
	forward.Y = 0;
	auto right = cameraTf.GetRight();
	right.Y = 0;

	Vector3 dir;

	if (_engine->Input().IsKeyDown(KEY_W))
		dir += forward;
	if (_engine->Input().IsKeyDown(KEY_S))
		dir -= forward;
	if (_engine->Input().IsKeyDown(KEY_A))
		dir -= right;
	if (_engine->Input().IsKeyDown(KEY_D))
		dir += right;
	dir = dir.GetNorm();

	_force += dir * MOVE_ACCEL * deltaTime;

	playerTf.Pos += _force * deltaTime;

	auto deltaRot = _force * ROT_K;
	playerTf.Rot.AddLocal(-deltaRot.X, deltaRot.Z, 0);
	
	//tf.Rot.Add(-dir.X, 0, 0);
	//tf.Rot.Add(0, dir.Z, 0);
}