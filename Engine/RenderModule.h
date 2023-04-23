#pragma once

#include "entt.hpp"
#include "RenderCameraDrawer.h"
#include "RenderImporter.h"
#include "RenderLightDrawer.h"
#include "RenderObjectDrawer.h"
#include "RenderShadowsDrawer.h"


namespace BlahEngine
{
class TimeModule;
class RenderShader;
class RenderBackground;

class IRenderModule
{
public:
	virtual void SetRenderBackground(RenderBackground&& background) = 0;

	virtual void AddRenderShader(RenderShader* shader) = 0;

	virtual void ImportModel(std::string fileName, float scaleFactor, RenderComp& render) = 0;
	virtual void ImportTexture(std::wstring fileName, RenderComp& render) = 0;

#if _DEBUG
	virtual void DebugSetShadowCasterView(bool isOn) = 0;
#endif
};

class RenderModule : virtual public IRenderModule
{
public:
	RenderModule();
	~RenderModule() noexcept(false);

	void Init(entt::registry* ecs, const TimeModule* timeModule, HWND hwnd, int screenWidth, int screenHeights);

	IDXGISwapChain* GetSwapChain() const;

	void SetRenderBackground(RenderBackground&& background) override;

	void AddRenderShader(RenderShader* shader) override;

	void ImportModel(std::string fileName, float scaleFactor, RenderComp& render) override;
	void ImportTexture(std::wstring fileName, RenderComp& render) override;

	void BeginFrame();
	void DrawFrame();
	void EndFrame();

#if _DEBUG
	void DebugSetShadowCasterView(bool isOn) override;
#endif

private:
	const TimeModule* _timeModule;
	entt::registry* _ecs;

	RenderImporter _importer;

	int _screenWidth;
	int _screenHeight;

	bool _isInited;

	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _context;
	ComPtr<IDXGISwapChain> _swapChain;
	ComPtr<ID3D11RenderTargetView> _renderTargetView;
	ComPtr<ID3D11DepthStencilView> _depthStencilView;
	ComPtr<ID3D11RasterizerState> _rasterizerState;
	D3D11_VIEWPORT _viewport;
	
	std::unique_ptr<RenderBackground> _backgroundDefault;
	std::unique_ptr<RenderBackground> _backgroundCustom;

	std::vector<std::unique_ptr<RenderShader>> _shaders;

	RenderCameraDrawer _cameraDrawer;
	RenderShadowsDrawer _shadowsDrawer;
	RenderLightDrawer _lightDrawer;
	RenderObjectDrawer _objectDrawer;


#if _DEBUG
	bool _debugIsShadowCasterView = false;
#endif

public:
	enum EShaderId
	{
		SimpleUnlit,
		SimpleLit,
		Unlit,
		Lit
	};
};
}
