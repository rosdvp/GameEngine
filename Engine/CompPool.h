#pragma once
#include "Entity.h"
#include "ArrayResize.h"

namespace BlahEngine
{
using CompId = unsigned int;

class ICompPool
{
public:
	virtual ~ICompPool() = default;

	virtual bool Has(Entity entity) = 0;
	virtual void Remove(Entity entity) = 0;

	virtual std::string GetCompName() const = 0;

protected:
	static CompId _nextCompId;

public:
#ifdef _DEBUG
	virtual void DebugPrint() const = 0;
#endif
};

template <typename T>
class CompPool : public ICompPool
{
public:
	static CompId GetId()
	{
		if (_id == 0)
		{
			if (_nextCompId == 0)
				_nextCompId = 1;
			_id = _nextCompId;
			_nextCompId *= 2;
		}
		return _id;
	}
private:
	static CompId _id;

public:
	CompPool(int baseCompSize, int entitiesSize):
		_compsSize(baseCompSize),
		_compsCount(0),
		_freeIndices(new int[baseCompSize]),
		_freeIndicesCount(0),
		_entityToCompIdx(new int[entitiesSize]),
		_entityToCompIdxSize(entitiesSize)
	{
		_comps = static_cast<T*>(malloc(sizeof(T) * baseCompSize));
		for (int i = 0; i < baseCompSize; i++)
			new(_comps + i) T();

		std::fill_n(_entityToCompIdx, _entityToCompIdxSize, -1);
	}

	CompPool(const CompPool<T>& pool) = delete;
	CompPool(CompPool<T>&& pool) = delete;
	CompPool& operator=(const CompPool& other) = delete;
	CompPool& operator=(CompPool&& other) noexcept = delete;

	~CompPool() override
	{
		for (int i = 0; i < _compsCount; i++)
			ReleaseComp(_comps[i]);
		free(_comps);
		delete[] _freeIndices;
		delete[] _entityToCompIdx;
	}

	T& Add(Entity entity)
	{
		ValidateEntitiesSize(entity);

		int compIdx = _entityToCompIdx[entity.Idx];
		if (compIdx != -1)
			throw std::exception("entity already has a comp");

		if (_freeIndicesCount > 0)
			compIdx = _freeIndices[--_freeIndicesCount];
		else
		{
			compIdx = _compsCount++;
			ValidateCompsSize();
		}
		_entityToCompIdx[entity.Idx] = compIdx;

		return _comps[compIdx];
	}

	T& Get(Entity entity)
	{
		if (entity.Idx >= _entityToCompIdxSize)
			throw std::exception("entity does not have a comp");

		int compIdx = _entityToCompIdx[entity.Idx];
		if (compIdx == -1)
			throw std::exception("entity does not have a comp");

		return _comps[compIdx];
	}

	bool Has(Entity entity) override
	{
		ValidateEntitiesSize(entity);

		return _entityToCompIdx[entity.Idx] != -1;
	}


	template <typename K, typename = int>
	struct HasRelease : std::false_type { };

	template <typename K>
	struct HasRelease <K, decltype(&K::Release, 0)> : std::true_type { };

	template<typename K>
	typename std::enable_if<HasRelease<K>::value>::type ReleaseComp(K& comp)
	{
		comp.Release();
	}
	template<typename K>
	typename std::enable_if<!HasRelease<K>::value>::type ReleaseComp(K& comp) { }

	void Remove(Entity entity) override
	{
		if (entity.Idx >= _entityToCompIdxSize)
			throw std::exception("entity does not have a comp");

		int compIdx = _entityToCompIdx[entity.Idx];
		if (compIdx == -1)
			throw std::exception("entity does not have a comp");

		ReleaseComp(_comps[compIdx]);
		_freeIndices[_freeIndicesCount++] = compIdx;
		_entityToCompIdx[entity.Idx] = -1;
	}


	std::string GetCompName() const override
	{
		auto replace = [](std::string& src, const std::string& from, const std::string& to)
		{
			auto start = src.find(from);
			if (start != std::string::npos)
				src.replace(start, from.length(), to);
		};

		std::string s0 = typeid(T).name();
		replace(s0, "struct ", "");
		replace(s0, "BlahEngine::", "");

		std::string s1 = " (";
		std::string s2 = std::to_string(GetId());
		std::string s3 = ")";
		return s0 + s1 + s2 + s3;
	}

private:
	T* _comps;
	int _compsSize;
	int _compsCount;

	int* _freeIndices;
	int _freeIndicesCount;

	int* _entityToCompIdx;
	int _entityToCompIdxSize;

	void ValidateCompsSize()
	{
		if (_compsCount < _compsSize)
			return;

		int newSize = _compsSize * 2;
		Math::ResizeByMalloc(&_comps, _compsSize, newSize);
		Math::ResizeByNewWithCopyAssign(&_freeIndices, _compsCount, newSize, -1);
		_compsSize = newSize;
	}

	void ValidateEntitiesSize(Entity entity)
	{
		if (entity.Idx < _entityToCompIdxSize)
			return;

		int newSize = entity.Idx * 2;
		Math::ResizeByNewWithCopyAssign(&_entityToCompIdx, _entityToCompIdxSize, newSize, -1);
		_entityToCompIdxSize = newSize;
	}

public:
#ifdef _DEBUG
	void DebugPrint() const override
	{
		using namespace std;

		cout << "-------comp pool " << GetCompName() << "-------" << endl;
		cout << "comps count: " << _compsCount << endl;
		cout << "free indices: ";
		for (int i = 0; i < _freeIndicesCount; i++)
			cout << _freeIndices[i] << ", ";
		cout << endl;
		cout << "entities: ";
		for (int i = 0; i < _entityToCompIdxSize; i++)
			if (_entityToCompIdx[i] != -1)
				cout << _entityToCompIdx[i] << ", ";
		cout << endl;
		cout << "---------------------" << endl;
	}
#endif
};

template<typename T>
CompId CompPool<T>::_id = 0;
}
