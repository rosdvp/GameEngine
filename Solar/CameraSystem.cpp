#include "CameraSystem.h"

using namespace BlahEngine;

CameraSystem::CameraSystem(float moveSpeed) :
    _moveSpeed(moveSpeed)
{}

void CameraSystem::Init()
{
    auto eCamera = _ecs->CreateEntity();
    auto& tf = _ecs->AddComp<TransformComp>(eCamera);
    tf.Pos = { 0, 0, -10 };
    auto& cameraComp = _ecs->AddComp<RenderCameraComp>(eCamera);
    cameraComp.IsOrthographic = false;
    cameraComp.PerspectiveAngle = 1.0f;
}

void CameraSystem::Run()
{
    float deltaTime = _engine->Time()->GetFrameDeltaSecs();
    float moveDelta = _moveSpeed * deltaTime;
    float rotDelta = 40 * deltaTime;

    auto& tf = _ecs->GetComp<TransformComp>(_eCamera);

    if (_engine->Input()->IsKeyDown(KEY_W))
        tf.Pos += tf.GetForward() * moveDelta;
    if (_engine->Input()->IsKeyDown(KEY_S))
        tf.Pos -= tf.GetForward() * moveDelta;
    if (_engine->Input()->IsKeyDown(KEY_A))
        tf.Pos -= tf.GetRight() * moveDelta;
    if (_engine->Input()->IsKeyDown(KEY_D))
        tf.Pos += tf.GetRight() * moveDelta;
    if (_engine->Input()->IsKeyDown(KEY_Q))
        tf.Pos += tf.GetUp() * moveDelta;
    if (_engine->Input()->IsKeyDown(KEY_E))
        tf.Pos -= tf.GetUp() * moveDelta;
    
    if (_engine->Input()->IsMouseMoved())
    {
        tf.Rot.Y += _engine->Input()->GetMouseDeltaX() * rotDelta;
        tf.Rot.X += _engine->Input()->GetMouseDeltaY() * rotDelta;
    }

    for (auto ev : _engine->Input()->GetEvents())
		if (ev.IsDown && ev.Key == KEY_C)
		{
            tf.Pos = { 0, 60, 0 };
            tf.Rot = { 90, 0, 0 };
		}
}