#include "pch.h"
#include "EntitiesPool.h"
#include "ArrayResize.h"

using namespace BlahEngine;

EntitiesPool::EntitiesPool(int baseSize) :
	_size(baseSize),
	_count(0),
	_existMap(new bool[baseSize]),
	_freeIndices(new int[baseSize]),
	_freeIndicesCount(0)
{
	std::fill_n(_existMap, baseSize, false);
}

EntitiesPool::~EntitiesPool()
{
	delete[] _existMap;
	delete[] _freeIndices;
}

Entity EntitiesPool::Create()
{
	Entity entity;
	if (_freeIndicesCount > 0)
		entity.Idx = _freeIndices[--_freeIndicesCount];
	else
	{
		entity.Idx = _count++;
		ValidateSize();
	}
	_existMap[entity.Idx] = true;
	return entity;
}

void EntitiesPool::Destroy(Entity entity)
{
	_existMap[entity.Idx] = false;
	_freeIndices[_freeIndicesCount++] = entity.Idx;
}

bool EntitiesPool::IsExist(Entity entity) const
{
	return _existMap[entity.Idx];
}

void EntitiesPool::ValidateSize()
{
	if (_count < _size)
		return;

	int newSize = _size * 2;
	Math::ResizeByNewWithCopyAssign(&_freeIndices, _size, newSize);
	Math::ResizeByNewWithCopyAssign(&_existMap, _size, newSize, false);
	_size = newSize;
}

#ifdef _DEBUG
void EntitiesPool::DebugPrint() const
{
	using namespace std;

	cout << "-------entities pool-------" << endl;
	cout << "alive: ";
	for (int i = 0; i < _size; i++)
		if (_existMap[i])
			cout << i << ", ";
	cout << endl;
	cout << "free indices: ";
	for (int i = 0; i < _freeIndicesCount; i++)
		cout << _freeIndices[i] << ", ";
	cout << endl;
	cout << "---------------------" << endl;
}
#endif