#pragma once

#include "BlahEngine.h"

using namespace BlahEngine;

class CameraSystem : public IInitSystem, IRunSystem
{
public:
	void Init() override
	{
		_entity = _ecs->CreateEntity();
		_ecs->AddComp<TransformComp>(_entity);
		auto& cameraComp = _ecs->AddComp<RenderCameraComp>(_entity);
		cameraComp.IsOrthographic = true;
		cameraComp.OrthoZoom = 30;
	}

	void Run() override
	{
		/*
		if (_engine->GetInputModule()->IsKeyDown(KEY_W))
			_ecs->AddOrGetComp<TransformComp>(_entity).Pos.Y += _moveSpeed;
		if (_engine->GetInputModule()->IsKeyDown(KEY_S))
			_ecs->AddOrGetComp<TransformComp>(_entity).Pos.Y -= _moveSpeed;
		if (_engine->GetInputModule()->IsKeyDown(KEY_A))
			_ecs->AddOrGetComp<TransformComp>(_entity).Pos.X -= _moveSpeed;
		if (_engine->GetInputModule()->IsKeyDown(KEY_D))
			_ecs->AddOrGetComp<TransformComp>(_entity).Pos.X += _moveSpeed;

		if (_engine->GetInputModule()->IsKeyDown(KEY_PLUS))
			_ecs->AddOrGetComp<RenderCameraComp>(_entity).OrthoZoom += _zoomSpeed;
		if (_engine->GetInputModule()->IsKeyDown(KEY_MINUS))
			_ecs->AddOrGetComp<RenderCameraComp>(_entity).OrthoZoom -= _zoomSpeed;
		*/
	}

private:
	Entity _entity {};
	float _moveSpeed = 0.001f;
	float _zoomSpeed = 0.1f;
};
