#pragma once

#include "EcsCore.h"
#include "UiTextComp.h"

namespace BlahEngine
{
class UiModule
{
public:
	UiModule();
	~UiModule();

	void Init(IDXGISwapChain* swapChain, EcsCore* ecs);

	void DrawFrame();

private:
	ComPtr<ID2D1Factory> _d2Factory;
	ComPtr<IDWriteFactory> _dwFactory;
	ComPtr<ID2D1RenderTarget> _renderTarget;
	ComPtr<ID2D1SolidColorBrush> _brushBlack;
	
	EcsCore* _ecs;
	Filter* _textsFilter;

	void SetupText(UiTextComp& textComp);
};
}
