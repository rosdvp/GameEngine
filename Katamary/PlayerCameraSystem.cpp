#include "PlayerCameraSystem.h"

#include "PlayerComp.h"

using namespace BlahEngine;

void PlayerCameraSystem::Init()
{
    _entCamera = _ecs->create();
    auto& tf = _ecs->emplace<TransformComp>(_entCamera);

    auto& cameraComp = _ecs->emplace<RenderCameraComp>(_entCamera);
    cameraComp.IsOrthographic = false;
    cameraComp.PerspectiveAngle = 0.7f;
}

void PlayerCameraSystem::Run()
{
	if (_entPlayer == entt::null)
	{
        auto view = _ecs->view<const PlayerComp>();
        if (!view.empty())
        {
            _entPlayer = view.front();
            auto& cameraTf = _ecs->get<TransformComp>(_entCamera);
            auto& playerTf = _ecs->get<TransformComp>(_entPlayer);
            cameraTf.Pos = playerTf.Pos + Vector3{0, POS_OFFSET_Y, -DIST_TO_PLAYER};
        }
        return;
	}
    float deltaTime = _engine->Time().GetFrameDeltaSecs();

    auto& cameraTf = _ecs->get<TransformComp>(_entCamera);
    auto& playerTf = _ecs->get<TransformComp>(_entPlayer);

    auto yaw = _engine->Input().GetMouseDeltaX() * SENSITIVITY * deltaTime;
    auto pitch = _engine->Input().GetMouseDeltaY() * SENSITIVITY * deltaTime;

    if (pitch + cameraTf.Rot.GetPitch() > 0)
        pitch = -cameraTf.Rot.GetPitch();
    if (pitch + cameraTf.Rot.GetPitch() < -85)
        pitch = -85 - cameraTf.Rot.GetPitch();

	cameraTf.Rot.AddLocal(0, pitch, 0);
    cameraTf.Rot.AddGlobal(0, 0, -yaw);

    Vector3 deltaPos = { 0, 0, -DIST_TO_PLAYER };
    deltaPos = deltaPos.Rotate(cameraTf.Rot.GetQuaternion());
    cameraTf.Pos = playerTf.Pos + deltaPos;
}