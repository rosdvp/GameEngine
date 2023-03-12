#include <iostream>

#include "BlahEngine.h"

#include "AnimatedRenderBackground.h"
#include "CameraSystem.h"
#include "MatrixRenderShader.h"
#include "SimpleRenderShader.h"

using namespace BlahEngine;

class InputDebugSystem : public IRunSystem
{
public:
	void Run() override
	{
		for (auto& ev : _engine->Input()->GetEvents())
		{
			std::cout << "key " << ev.Key << " " << (ev.IsDown ? "down" : "up") << std::endl;
		}
	}
};


class TestSystem : public IInitSystem, IRunSystem
{
public:
	Entity _squareA;
	Entity _squareB;

	float _moveSpeed = 0.01f;

	void Init() override
	{
		_squareA = CreateSquare({ -2, 0, 0 });
		_squareB = CreateSquare({ 2, 0, 0 });
	}

	void Run() override
	{
		if (_engine->Input()->IsKeyDown(KEY_W))
			_engine->Physics()->TryMove(_squareA, { 0, _moveSpeed, 0 });
		if (_engine->Input()->IsKeyDown(KEY_S))
			_engine->Physics()->TryMove(_squareA, { 0, -_moveSpeed, 0 });
		if (_engine->Input()->IsKeyDown(KEY_A))
			_engine->Physics()->TryMove(_squareA, { -_moveSpeed, 0, 0 });
		if (_engine->Input()->IsKeyDown(KEY_D))
			_engine->Physics()->TryMove(_squareA, { _moveSpeed, 0, 0 });

		if (_engine->Input()->IsKeyDown(KEY_I))
			_engine->Physics()->TryMove(_squareB, { 0, _moveSpeed, 0 });
		if (_engine->Input()->IsKeyDown(KEY_K))
			_engine->Physics()->TryMove(_squareB, { 0, -_moveSpeed, 0 });
		if (_engine->Input()->IsKeyDown(KEY_J))
			_engine->Physics()->TryMove(_squareB, { -_moveSpeed, 0, 0 });
		if (_engine->Input()->IsKeyDown(KEY_L))
			_engine->Physics()->TryMove(_squareB, { _moveSpeed, 0, 0 });
	}
	
	Entity CreateSquare(const Vector3& pos)
	{
		auto entity = _ecs->CreateEntity();
		
		_ecs->AddComp<TransformComp>(entity).Pos = pos;
		_ecs->AddComp<BoxCollisionComp>(entity);

		auto& renderComp = _ecs->AddComp<RenderComp>(entity);
		renderComp.VerticesCount = 4;
		renderComp.Vertices = new RenderComp::Vertex[renderComp.VerticesCount]
		{
		{DirectX::XMFLOAT3(-1.0f, 1.0f, 0.5f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{DirectX::XMFLOAT3(1.0f, 1.0f, 0.5f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{DirectX::XMFLOAT3(1.0f, -1.0f, 0.5f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{DirectX::XMFLOAT3(-1.0f, -1.0f, 0.5f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		};
		renderComp.IndicesCount = 6;
		renderComp.Indices = new int[renderComp.IndicesCount]
		{
			0, 3, 1,
			1, 3, 2
		};
		renderComp.ShaderId = 1;
		renderComp.DrawerId = 0;

		return entity;
	}
};


int main()
{
	auto engine = new Engine();

	auto simpleShader = new SimpleRenderShader();
	auto matrixShader = new MatrixRenderShader();
	engine->Render()->AddRenderShader(simpleShader);
	engine->Render()->AddRenderShader(matrixShader);

	engine->Gameplay()->AddSystem(new CameraSystem());
	engine->Gameplay()->AddSystem(new TestSystem());
		
	engine->Init();
	engine->Run();

	delete engine;

	return 0;
}