#pragma once

#include "entt.hpp"
#include "RenderCameraDrawer.h"
#include "RenderImporter.h"
#include "RenderLightDrawer.h"


namespace BlahEngine
{
class RenderDrawer;
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

	void BeginDrawFrame();
	void DrawFrame();
	void EndDrawFrame();

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
	
	std::unique_ptr<RenderBackground> _renderBackgroundDefault;
	std::unique_ptr<RenderBackground> _renderBackgroundCustom;

	std::vector<std::unique_ptr<RenderShader>> _renderShaders;
	std::vector<std::unique_ptr<RenderDrawer>> _renderDrawers;

	RenderCameraDrawer _cameraDrawer;
	RenderLightDrawer _lightDrawer;

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
