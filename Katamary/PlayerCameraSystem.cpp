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
	        _entPlayer = view.front();
        return;
	}
    float deltaTime = _engine->Time().GetFrameDeltaSecs();

    auto& cameraTf = _ecs->get<TransformComp>(_entCamera);
    auto& playerTf = _ecs->get<TransformComp>(_entPlayer);

    float deltaYaw = _engine->Input().GetMouseDeltaX() * SENSITIVITY_ROTATE * deltaTime;
    float deltaPitch = -_engine->Input().GetMouseDeltaY() * SENSITIVITY_ROTATE * deltaTime;
    deltaPitch = ClampPitchDelta(cameraTf.Rot.GetPitch(), deltaPitch);
    //return;

    cameraTf.Rot.AddAroundLocal(0, deltaPitch, 0);
    cameraTf.Rot.AddAroundWorld(0, 0, deltaYaw);

    float deltaZoom = _engine->Input().GetMouseWheelDelta() * SENSITIVITY_ZOOM * deltaTime;
    _distToPlayer -= deltaZoom;

    if (deltaYaw != 0 || deltaPitch != 0)
    {
        //std::cout << cameraTf.Rot.GetPitch() << std::endl;
        //_engine->Stats().OutputStats();
    }

    Vector3 deltaPos = { 0, 0, -_distToPlayer };
    deltaPos = deltaPos.Rotate(cameraTf.Rot.GetQuaternion());
    cameraTf.Pos = playerTf.Pos + deltaPos;
}

float PlayerCameraSystem::ClampPitchDelta(float currPitch, float deltaPitch)
{
    float rangeAmin = -175;
    float rangeAmax = -180 + 85;
    float rangeBmin = 5;
    float rangeBmax = 85;

    if (currPitch >= rangeAmin - 1 && currPitch <= rangeAmax + 1)
    {
        if (currPitch + deltaPitch < rangeAmin)
            deltaPitch = rangeAmin - currPitch;
        if (currPitch + deltaPitch > rangeAmax)
            deltaPitch = rangeAmax - currPitch;
    }
    if (currPitch >= rangeBmin - 1 && currPitch <= rangeBmax + 1)
    {
        if (currPitch + deltaPitch < rangeBmin)
            deltaPitch = rangeBmin - currPitch;
        if (currPitch + deltaPitch > rangeBmax)
            deltaPitch = rangeBmax - currPitch;
    }

    return deltaPitch;
}
