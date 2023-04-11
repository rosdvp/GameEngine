#pragma once
#include <functional>

namespace BlahEngine
{
    template <typename T, typename... Others>
    void HashCombine(std::size_t& seed, const T& v, const Others&... others)
    {
        seed ^= std::hash<T>{}(v)+0x9e3779b9 + (seed << 6) + (seed >> 2);
        (HashCombine(seed, others), ...);
    }
}