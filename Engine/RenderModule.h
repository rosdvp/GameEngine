#pragma once

#include "entt.hpp"
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
	virtual void SetRenderBackground(RenderBackground&& background) = 0;
	virtual void AddRenderShader(RenderShader&& shader) = 0;
};

class RenderModule : virtual public IRenderModule
{
public:
	RenderModule();
	~RenderModule() noexcept(false);

	void Init(entt::registry* ecs, const TimeModule* timeModule, HWND hwnd, int screenWidth, int screenHeights);

	IDXGISwapChain* GetSwapChain() const;

	void BeginDrawFrame();
	void DrawFrame();
	void EndDrawFrame();

	void SetRenderBackground(RenderBackground&& background) override;
	void AddRenderShader(RenderShader&& shader) override;

private:
	const TimeModule* _timeModule;
	entt::registry* _ecs;

	int _screenWidth;
	int _screenHeight;

	bool _isInited;

	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _context;
	ComPtr<IDXGISwapChain> _swapChain;
	ComPtr<ID3D11RenderTargetView> _renderTargetView;
	ComPtr<ID3D11DepthStencilView> _depthStencilView;
	ComPtr<ID3D11RasterizerState> _rasterizerState;
	
	std::unique_ptr<RenderBackground> _renderBackgroundDefault;
	std::unique_ptr<RenderBackground> _renderBackgroundCustom;

	std::vector<std::unique_ptr<RenderDrawer>> _renderDrawers;
	std::vector<std::unique_ptr<RenderShader>> _renderShaders;

	void DrawCamera();
};
}
