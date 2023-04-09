// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#pragma once

#include <clocale>
#include <ctime>

#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <iostream>
#include <memory>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>
#include "DirectXMath.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

#include <wrl.h>
using namespace Microsoft::WRL;

#include "entt.hpp"