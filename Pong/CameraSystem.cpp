#include "CameraSystem.h"

using namespace BlahEngine;

void CameraSystem::Init()
{
	auto ent = _ecs->create();
	auto& tf =_ecs->emplace<TransformComp>(ent);
	tf.Pos = { 0, 0, -10 };
	auto& cameraComp = _ecs->emplace<RenderCameraComp>(ent);
	cameraComp.IsOrthographic = true;
	cameraComp.OrthoZoom = 30;
}