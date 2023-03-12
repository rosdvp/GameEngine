#include "pch.h"
#include "Filter.h"

#include "EcsCore.h"

using namespace BlahEngine;

Filter::Filter(EcsCore* core, std::vector<CompId> incCompsIds, std::vector<CompId> excCompsIds,
	int baseFilteredEntitiesSize, int baseEntitiesMapSize) :
	_core(core),
	_filteredEntities(new Entity[baseFilteredEntitiesSize]),
	_filteredEntitiesSize(baseFilteredEntitiesSize),
	_filteredEntitiesCount(0),
	_entityToFilteredIdxMap(new int[baseEntitiesMapSize]),
	_entityToFilteredIdxSize(baseFilteredEntitiesSize),
	_incCompsIds(std::move(incCompsIds)),
	_excCompsIds(std::move(excCompsIds)),
	_goingIteratingsCount(0)
{
	std::fill_n(_entityToFilteredIdxMap, baseEntitiesMapSize, -1);
}

Filter::~Filter() {}

void Filter::ValidateEntity(Entity entity)
{
	if (_goingIteratingsCount > 0)
	{
		_delayedValidations.push_back(entity);
		return;
	}

	ValidateEntitiesMapSize(entity);

	bool isFit = _core->IsEntityExist(entity);
	if (isFit)
		for (auto id : _incCompsIds)
			if (!_core->HasComp(entity, id))
			{
				isFit = false;
				break;
			}
	if (isFit)
		for (auto id : _excCompsIds)
			if (_core->HasComp(entity, id))
			{
				isFit = false;
				break;
			}

	if (isFit && _entityToFilteredIdxMap[entity.Idx] == -1)
	{
		_filteredEntities[_filteredEntitiesCount++] = entity;
		_entityToFilteredIdxMap[entity.Idx] = _filteredEntitiesCount - 1;
		ValidateFilteredEntitiesSize();
	}
	else if (!isFit && _entityToFilteredIdxMap[entity.Idx] != -1)
	{
		if (_filteredEntitiesCount == 1)
		{
			_filteredEntitiesCount = 0;
		}
		else
		{
			int filteredIdx = _entityToFilteredIdxMap[entity.Idx];
			Entity last = _filteredEntities[--_filteredEntitiesCount];
			_entityToFilteredIdxMap[last.Idx] = filteredIdx;
			_filteredEntities[filteredIdx] = last;
		}
		_entityToFilteredIdxMap[entity.Idx] = -1;
	}
}

void Filter::BeginIteration()
{
	_goingIteratingsCount++;
}
void Filter::EndIteration()
{
	_goingIteratingsCount--;

	for (auto entity : _delayedValidations)
		ValidateEntity(entity);
	_delayedValidations.clear();
}

Entity* Filter::begin()
{
	return _filteredEntities;
}
Entity* Filter::end()
{
	return _filteredEntities + _filteredEntitiesCount;
}
Entity* Filter::cbegin() const
{
	return _filteredEntities;
}
Entity* Filter::cend() const
{
	return _filteredEntities + _filteredEntitiesCount;
}
Entity* Filter::begin() const
{
	return _filteredEntities;
}
Entity* Filter::end() const
{
	return _filteredEntities + _filteredEntitiesCount;
}


void Filter::ValidateEntitiesMapSize(Entity entity)
{
	if (entity.Idx < _entityToFilteredIdxSize)
		return;

	int newSize = _entityToFilteredIdxSize * 2;
	Math::ResizeByNewWithCopyAssign(&_entityToFilteredIdxMap, _entityToFilteredIdxSize, newSize, -1);
	_entityToFilteredIdxSize = newSize;
}

void Filter::ValidateFilteredEntitiesSize()
{
	if (_filteredEntitiesCount < _filteredEntitiesSize)
		return;
	int newSize = _filteredEntitiesSize * 2;
	Math::ResizeByNewWithCopyAssign(&_filteredEntities, _filteredEntitiesSize, newSize);
	_filteredEntitiesSize = newSize;
}

#ifdef _DEBUG
void Filter::DebugPrint() const
{
	using namespace std;
	cout << "-------filter-------" << endl;
	cout << "inc comps ids: ";
	for (auto id : _incCompsIds)
	{
		auto pool = _core->GetCompPoolById(id);
		if (pool == nullptr)
			cout << "unknown(" << id << "), ";
		else
 			cout << pool->GetCompName() << ", ";
	}
	cout << endl;
	cout << "exc comps ids: ";
	for (auto id : _excCompsIds)
	{
		auto pool = _core->GetCompPoolById(id);
		if (pool == nullptr)
			cout << "unknown(" << id << "), ";
		else
			cout << pool->GetCompName() << ", ";
	}
	cout << endl;
	cout << "entities: ";
	for (int i = 0; i < _filteredEntitiesCount; i++)
		cout << _filteredEntities[i].Idx << ", ";
	cout << endl;
	cout << "---------------------" << endl;
}
#endif
