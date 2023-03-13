#pragma once

namespace BlahEngine
{
namespace Math
{
template <typename T>
void ResizeByNewWithCopyAssign(T** src, int oldSize, int newSize)
{
	T* temp = new T[newSize];
	std::copy_n(*src, oldSize, temp);
	delete[] * src;
	*src = temp;
}

template<typename T>
void ResizeByNewWithCopyAssign(T** src, int oldSize, int newSize, T&& defaultValue)
{
	T* temp = new T[newSize];
	std::copy_n(*src, oldSize < newSize ? oldSize : newSize, temp);
	std::fill_n(temp + oldSize, newSize - oldSize, defaultValue);
	delete[] * src;
	*src = temp;
}

template <typename T>
void ResizeByMalloc(T** src, int oldSize, int newSize)
{
	T* temp = static_cast<T*>(malloc(sizeof(T) * newSize));

	for (int i = 0; i < oldSize; i++)
		new (temp + i) T(std::move((*src)[i]));
	for (int i = oldSize; i < newSize; i++)
		new (temp + i) T();

	free(*src);
	
	*src = temp;
}
}
}
