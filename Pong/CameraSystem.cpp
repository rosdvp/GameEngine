#include "CameraSystem.h"

using namespace BlahEngine;

void CameraSystem::Init()
{
	Entity entity = _ecs->CreateEntity();
	_ecs->AddComp<TransformComp>(entity);
	auto& cameraComp = _ecs->AddComp<RenderCameraComp>(entity);
	cameraComp.IsOrthographic = true;
	cameraComp.OrthoZoom = 30;
}