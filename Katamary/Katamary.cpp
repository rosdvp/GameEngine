#include "BlahEngine.h"
#include "GeometryRenderBuilder.h"
#include "LevelSystem.h"
#include "PlayerCameraSystem.h"
#include "PlayerSystem.h"
#include "RenderLightComp.h"

using namespace BlahEngine;

class TestSystem : public IInitSystem, IRunSystem
{
public:
	entt::entity _entGizmoForward;
	entt::entity _entGizmoRight;
	entt::entity _entGizmoUp;

	void Init() override
	{
		{
			_entGizmoForward = _ecs->create();
			auto& tf = _ecs->emplace<TransformComp>(_entGizmoForward);
			tf.Scale = { 0.3, 0.3, 0.3 };

			auto& render = _ecs->emplace<RenderComp>(_entGizmoForward);
			render.ShaderId = RenderModule::SimpleUnlit;
			GeometryRenderBuilder::BuildCube(render, Color::White());
		}
		{
			_entGizmoRight = _ecs->create();
			auto& tf = _ecs->emplace<TransformComp>(_entGizmoRight);
			tf.Scale = { 0.3, 0.3, 0.3 };

			auto& render = _ecs->emplace<RenderComp>(_entGizmoRight);
			render.ShaderId = RenderModule::SimpleUnlit;
			GeometryRenderBuilder::BuildCube(render, Color::Purple());
		}
		{
			_entGizmoUp = _ecs->create();
			auto& tf = _ecs->emplace<TransformComp>(_entGizmoUp);
			tf.Scale = { 0.3, 0.3, 0.3 };

			auto& render = _ecs->emplace<RenderComp>(_entGizmoUp);
			render.ShaderId = RenderModule::SimpleUnlit;
			GeometryRenderBuilder::BuildCube(render, Color::Orange());
		}
	}

	void Run() override
	{
		float deltaTime = _engine->Time().GetFrameDeltaSecs();

		auto view = _ecs->view<TransformComp, RenderLightComp>();

		for (auto ent : view)
		{
			float moveSpeed = 1 * deltaTime;
			float rotSpeed = 40 * deltaTime;

			auto [tf, light] = view.get(ent);

			if (_engine->Input().IsKeyDown(KEY_UP))
				tf.Pos += tf.GetForward() * moveSpeed;
			if (_engine->Input().IsKeyDown(KEY_DOWN))
				tf.Pos -= tf.GetForward() * moveSpeed;
			if (_engine->Input().IsKeyDown(KEY_RIGHT))
				tf.Pos += tf.GetRight() * moveSpeed;
			if (_engine->Input().IsKeyDown(KEY_LEFT))
				tf.Pos -= tf.GetRight() * moveSpeed;

			if (_engine->Input().IsKeyDown(KEY_I))
				//tf.Rot.AddPitch(rotSpeed);
				tf.Rot.AddAroundLocal(0, rotSpeed, 0);
			if (_engine->Input().IsKeyDown(KEY_K))
				//tf.Rot.AddPitch(-rotSpeed);
				tf.Rot.AddAroundLocal(0, -rotSpeed, 0);
			if (_engine->Input().IsKeyDown(KEY_L))
				//tf.Rot.AddYaw(rotSpeed);
				tf.Rot.AddAroundWorld(0, 0, rotSpeed);
			if (_engine->Input().IsKeyDown(KEY_J))
				//tf.Rot.AddYaw(-rotSpeed);
				tf.Rot.AddAroundWorld(0, 0, -rotSpeed);

			if (_engine->Input().IsKeyPressed(KEY_T))
				_engine->Render().DebugSetShadowCasterView(true);
			if (_engine->Input().IsKeyPressed(KEY_G))
				_engine->Render().DebugSetShadowCasterView(false);

			tf.Pos = Vector3{ 0, 0, -1 }.Rotate(tf.Rot.GetQuaternion()) * 30;
			//tf.Pos = { 0, 0, -10 };

			//_ecs->get<TransformComp>(_entGizmoForward).Pos = tf.Pos + tf.GetForward();
			//_ecs->get<TransformComp>(_entGizmoRight).Pos = tf.Pos + tf.GetRight();
			//_ecs->get<TransformComp>(_entGizmoUp).Pos = tf.Pos + tf.GetUp();

			//std::cout << tf.Rot.GetEuler() << std::endl;
		}
	}
};


int main()
{
	Engine engine;
	engine.Init();
	
	engine.Systems().AddSystem(new LevelSystem);
	engine.Systems().AddSystem(new PlayerSystem);
	engine.Systems().AddSystem(new PlayerCameraSystem);
	engine.Systems().AddSystem(new TestSystem);

	engine.Run();

	return 0;
}