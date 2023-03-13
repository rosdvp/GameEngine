#include "CameraSystem.h"

using namespace BlahEngine;

void CameraSystem::Init()
{
	Entity entity = _ecs->CreateEntity();
	auto& tf =_ecs->AddComp<TransformComp>(entity);
	tf.Pos = { 0, 0, -10 };
	auto& cameraComp = _ecs->AddComp<RenderCameraComp>(entity);
	cameraComp.IsOrthographic = true;
	cameraComp.OrthoZoom = 30;
}