#pragma once

#include "EcsCore.h"
#include "RenderBackground.h"
#include "RenderCameraComp.h"
#include "RenderDrawer.h"
#include "RenderShader.h"
#include "TimeModule.h"


namespace BlahEngine
{

class IRenderModule
{
public:
	virtual void SetRenderBackground(RenderBackground* background) = 0;
	virtual void AddRenderShader(RenderShader* shader) = 0;
};

class RenderModule : virtual public IRenderModule
{
public:
	RenderModule();
	~RenderModule() noexcept(false);

	void Init(HWND hwnd, int screenWidth, int screenHeight, const TimeModule* timeModule, EcsCore* ecs);

	IDXGISwapChain* GetSwapChain() const;

	void BeginDrawFrame();
	void DrawFrame();
	void EndDrawFrame();

	void SetRenderBackground(RenderBackground* background) override;
	void AddRenderShader(RenderShader* shader) override;

private:
	const TimeModule* _timeModule;
	EcsCore* _ecs;

	int _screenWidth;
	int _screenHeight;

	bool _isInited;

	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _context;
	ComPtr<IDXGISwapChain> _swapChain;
	ComPtr<ID3D11RenderTargetView> _renderTargetView;
	ComPtr<ID3D11DepthStencilView> _depthStencilView;
	ComPtr<ID3D11RasterizerState> _rasterizerState;
	
	RenderBackground* _renderBackgroundDefault;
	RenderBackground* _renderBackgroundCustom;

	RenderDrawer _renderDrawer;
	std::vector<RenderShader*> _renderShaders;

	Filter* _cameraFilter;
	Filter* _rendersFilter;

	void DrawCamera(const TransformComp& transComp, RenderCameraComp& cameraComp);
};
}
