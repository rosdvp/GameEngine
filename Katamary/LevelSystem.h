#pragma once

#include "BlahEngine.h"

class LevelSystem : public BlahEngine::IInitSystem, BlahEngine::IRunSystem
{
public:
	void Init() override;

	void Run() override;

private:
	entt::entity _entDebugBox {entt::null};

	void CreateGround();

	entt::entity CreateCube(
		const BlahEngine::Vector3& pos, 
		const BlahEngine::Rotation& rot, 
		const BlahEngine::Vector3& scale
	);

	void CreateDuck(const BlahEngine::Vector3& pos, const BlahEngine::Rotation& rot, float scale);
	void CreateCar(const BlahEngine::Vector3& pos, const BlahEngine::Rotation& rot, float scale);


	entt::entity CreateModel(
		const BlahEngine::Vector3& pos, 
		const BlahEngine::Rotation& rot, 
		float scale,
		const std::string& pathToModel,
		const std::wstring& pathToTexture
	);
};