#pragma once

namespace BlahEngine
{
class TransformsModule
{
public:
	TransformsModule();
	~TransformsModule();

	void Init(entt::registry* ecs);

	void AdjustGlobalPositions();
	void ResetTransformsIsChanged();

private:
	entt::registry* _ecs;

	bool _isSortingRequired;

	void OnTransformAddedOrRemoved(entt::registry& reg, entt::entity ent);
};
}
