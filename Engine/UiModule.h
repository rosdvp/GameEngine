#pragma once

#include "entt.hpp"

#include "UiTextComp.h"

namespace BlahEngine
{
class UiModule
{
public:
	UiModule();
	~UiModule();

	void Init(entt::registry* ecs, IDXGISwapChain* swapChain);

	void DrawFrame();

private:
	entt::registry * _ecs;

	ComPtr<ID2D1Factory> _d2Factory;
	ComPtr<IDWriteFactory> _dwFactory;
	ComPtr<ID2D1RenderTarget> _renderTarget;
	ComPtr<ID2D1SolidColorBrush> _brushBlack;

	void SetupText(UiTextComp& textComp);
};
}
