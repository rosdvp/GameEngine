#pragma once
#include "Entity.h"
#include "CompPool.h"


namespace BlahEngine
{
class EcsCore;

class Filter
{
public:
	Filter(EcsCore* core, std::vector<CompId> incCompsIds, std::vector<CompId> excCompsIds,
		int baseFilteredEntitiesSize, int baseEntitiesMapSize);
	~Filter();

	Filter(const Filter& set) = delete;
	Filter(Filter&& set) = delete;

	Filter& operator=(const Filter& other) = delete;
	Filter& operator=(Filter&& other) noexcept = delete;

	void ValidateEntity(Entity entity);

	void BeginIteration();
	void EndIteration();

	Entity* begin();
	Entity* end();
	Entity* cbegin() const;
	Entity* cend() const;
	Entity* begin() const;
	Entity* end() const;

private:
	EcsCore* _core;

	Entity* _filteredEntities;
	int _filteredEntitiesSize;
	int _filteredEntitiesCount;

	int* _entityToFilteredIdxMap;
	int _entityToFilteredIdxSize;

	std::vector<CompId> _incCompsIds;
	std::vector<CompId> _excCompsIds;

	int _goingIteratingsCount;
	std::vector<Entity> _delayedValidations;


	void ValidateEntitiesMapSize(Entity entity);
	void ValidateFilteredEntitiesSize();

public:
#ifdef _DEBUG
	void DebugPrint() const;
#endif
};
}