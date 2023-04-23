#include "FreeCameraSystem.h"

using namespace BlahEngine;

void FreeCameraSystem::Init()
{
    _entCamera = _ecs->create();
    auto& tf = _ecs->emplace<TransformComp>(_entCamera);
    SetDefaultPos(tf);

    auto& cameraComp = _ecs->emplace<RenderCameraComp>(_entCamera);
    cameraComp.IsOrthographic = false;
    cameraComp.PerspectiveAngle = 0.7f;
}

void FreeCameraSystem::Run()
{
    float deltaTime = _engine->Time().GetFrameDeltaSecs();
    float moveDelta = _moveSpeed * deltaTime;
    float rotDelta = 40 * deltaTime;

    if (_engine->Input().IsKeyDown(KEY_SHIFT))
        moveDelta *= 5;

    auto& tf = _ecs->get<TransformComp>(_entCamera);

    
    if (_engine->Input().IsKeyDown(KEY_W))
        tf.Pos += tf.GetForward() * moveDelta;
    if (_engine->Input().IsKeyDown(KEY_S))
        tf.Pos -= tf.GetForward() * moveDelta;
    if (_engine->Input().IsKeyDown(KEY_A))
        tf.Pos -= tf.GetRight() * moveDelta;
    if (_engine->Input().IsKeyDown(KEY_D))
        tf.Pos += tf.GetRight() * moveDelta;
    if (_engine->Input().IsKeyDown(KEY_Q))
        tf.Pos += tf.GetUp() * moveDelta;
    if (_engine->Input().IsKeyDown(KEY_E))
        tf.Pos -= tf.GetUp() * moveDelta;


    if (_engine->Input().IsMouseMoved())
    {
        float yawDelta = _engine->Input().GetMouseDeltaX() * rotDelta;
        float pitchDelta = _engine->Input().GetMouseDeltaY() * rotDelta;

        tf.Rot.AddAroundLocal(0, pitchDelta, yawDelta);
    }

    if (_engine->Input().IsKeyPressed(KEY_C))
    {
        SetDefaultPos(tf);
    }
}

void FreeCameraSystem::SetDefaultPos(TransformComp& tf)
{
    tf.Pos = { 0, 10, -80 };
    tf.Rot = { 0, 0, 0 };
}
