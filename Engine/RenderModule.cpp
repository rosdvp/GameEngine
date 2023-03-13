#include "pch.h"
#include "RenderModule.h"

#include "Logger.h"
#include "RenderCameraComp.h"
#include "RenderComp.h"

using namespace BlahEngine;
using namespace DirectX;

RenderModule::RenderModule() :
	_timeModule(nullptr),
	_ecs(nullptr),
	_screenWidth(0),
	_screenHeight(0),
	_isInited(false),
	_renderBackgroundCustom(nullptr),
	_cameraFilter(nullptr),
	_rendersFilter(nullptr)
{
	_renderBackgroundDefault = new RenderBackground();
	_renderShaders.push_back(new RenderShader);
}

RenderModule::~RenderModule() noexcept(false)
{
	for (auto s : _renderShaders)
		delete s;
	delete _renderBackgroundDefault;

	Logger::Debug("render module", "destroyed");
}


void RenderModule::Init(HWND hwnd, int screenWidth, int screenHeight, 
	const TimeModule* timeModule, EcsCore* ecs)
{
	Logger::Debug("render module", "initializing");

	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_timeModule = timeModule;
	_ecs = ecs;

	HRESULT result = S_OK;

	int createDeviceFlags = 0;
	createDeviceFlags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT; //this flag is created 
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	int driverTypesCount = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	int featureLevelsCount = ARRAYSIZE(featureLevels);

	//--------------------------------------------------
	//front buffer description
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	//--------------------------------------------------
	//searching for driver
	for (int driverTypeIndex = 0; driverTypeIndex < driverTypesCount; driverTypeIndex++)
	{
		auto driverType = driverTypes[driverTypeIndex];

		D3D_FEATURE_LEVEL featureLevel;

		result = D3D11CreateDeviceAndSwapChain(
			nullptr,
			driverType,
			nullptr,
			createDeviceFlags,
			featureLevels,
			featureLevelsCount,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			_swapChain.GetAddressOf(),
			_device.GetAddressOf(),
			&featureLevel,
			_context.GetAddressOf());
		if (SUCCEEDED(result))
			break;
	}
	if (FAILED(result))
		throw std::exception("failed to create swap chain");
	Logger::Debug("render module", "swap chain is created");

	//--------------------------------------------------
	//creating back buffer
	ID3D11Texture2D* backBuffer = nullptr;
	result = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(result))
		throw std::exception("failed to create back buffer");
	Logger::Debug("render module", "back buffer is created");

	//create render target to which drawing will happen (it is a back buffer)
	result = _device->CreateRenderTargetView(backBuffer, nullptr, _renderTargetView.GetAddressOf());
	if (FAILED(result))
		throw std::exception("failed to create render target view");

	backBuffer->Release();
	backBuffer = nullptr;

	Logger::Debug("render module", "render target is created");

	//--------------------------------------------------
	//creating depth stencil view (which is necessary for 3d objects render)

	D3D11_TEXTURE2D_DESC depthStencilDesc {};
	depthStencilDesc.Width = screenWidth;
	depthStencilDesc.Height = screenHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	ID3D11Texture2D* depthStencilBuffer;
	result = _device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer);
	if (FAILED(result))
		throw std::exception("failed to create depth stencil buffer");

	result = _device->CreateDepthStencilView(depthStencilBuffer, nullptr, _depthStencilView.GetAddressOf());
	if (FAILED(result))
		throw std::exception("failed to create depth stencil view");
	depthStencilBuffer->Release();
	depthStencilBuffer = nullptr;

	//--------------------------------------------------
	//create rasterizer state that defines what should be culled (without this clockwise indices order will not be render)
	CD3D11_RASTERIZER_DESC rasterizerStateDesc = {};
	rasterizerStateDesc.CullMode = D3D11_CULL_BACK;
	rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
	
	result = _device->CreateRasterizerState(&rasterizerStateDesc, _rasterizerState.GetAddressOf());
	if (FAILED(result))
		throw std::exception("failed to create rasterizer state");

	Logger::Debug("render module", "rasterizer state is created");

	//--------------------------------------------------
	//compiling shaders

	for (auto s : _renderShaders)
		s->Init(_device.Get());

	//--------------------------------------------------
	//initializing default background
	_renderBackgroundDefault->Init(_device.Get(), _timeModule);

	//--------------------------------------------------
	//filling render frame dto

	_cameraFilter = ecs->GetFilter(FilterMask().Inc<TransformComp>().Inc<RenderCameraComp>());
	_rendersFilter = ecs->GetFilter(FilterMask().Inc<TransformComp>().Inc<RenderComp>());

	_isInited = true;
	Logger::Debug("render module", "initialized");
}

IDXGISwapChain* RenderModule::GetSwapChain() const
{
	return _swapChain.Get();
}

void RenderModule::BeginDrawFrame()
{
	_context->ClearState();

	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(_screenWidth);
	viewport.Height = static_cast<float>(_screenHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	_context->RSSetViewports(1, &viewport);

	_context->OMSetRenderTargets(
		1,
		_renderTargetView.GetAddressOf(),
		_depthStencilView.Get()
	);
	_context->RSSetState(_rasterizerState.Get());

	_context->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void RenderModule::DrawFrame()
{
	(_renderBackgroundCustom != nullptr
		? _renderBackgroundCustom
		: _renderBackgroundDefault)
		->DrawFrame(_context.Get(), _renderTargetView.Get());

	for (auto entity : *_cameraFilter)
	{
		auto& transComp = _ecs->GetComp<TransformComp>(entity);
		auto& cameraComp = _ecs->GetComp<RenderCameraComp>(entity);
		DrawCamera(transComp, cameraComp);
	}

	for (auto entity : *_rendersFilter)
	{
		auto& transComp = _ecs->GetComp<TransformComp>(entity);
		auto& renderComp = _ecs->GetComp<RenderComp>(entity);

		auto shader = _renderShaders[renderComp.ShaderId];
		shader->ApplyToContext(_context.Get());

		_renderDrawer.DrawEntity(_device.Get(), _context.Get(), transComp, renderComp);
	}
}

void RenderModule::EndDrawFrame()
{
	_context->OMSetRenderTargets(0, nullptr, nullptr);
	_swapChain->Present(1, 0);
}


void RenderModule::SetRenderBackground(RenderBackground* background)
{
	if (!_isInited)
		throw std::exception("render module is not initialized");

	background->Init(_device.Get(), _timeModule);
	_renderBackgroundCustom = background;
}

void RenderModule::AddRenderShader(RenderShader* shader)
{
	_renderShaders.push_back(shader);
}


void RenderModule::DrawCamera(const TransformComp& transComp, RenderCameraComp& cameraComp)
{
	bool shouldUpdateBuffer = false;
	if (cameraComp.ConstantBuffer.Get() == nullptr)
	{
		D3D11_BUFFER_DESC constantBufferDesc = {};
		constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		constantBufferDesc.ByteWidth = sizeof(XMMATRIX);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = 0;

		HRESULT result = _device->CreateBuffer(
			&constantBufferDesc,
			nullptr,
			cameraComp.ConstantBuffer.GetAddressOf());
		if (FAILED(result))
			throw std::exception("failed to create camera constant buffer");

		shouldUpdateBuffer = true;
	}

	if (shouldUpdateBuffer || transComp.IsChanged() || cameraComp.IsChanged())
	{
		XMMATRIX finalMatrix;
		if (cameraComp.IsOrthographic)
		{
			auto vectEye = XMVectorSet(transComp.Pos.X, transComp.Pos.Y, transComp.Pos.Z, 0.0f);
			auto vectAt = XMVectorSet(transComp.Pos.X, transComp.Pos.Y, 0.0f, 0.0f);
			auto vectUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			XMMATRIX viewMatrix = XMMatrixLookAtLH(vectEye, vectAt, vectUp);

			float width = max(_screenWidth / cameraComp.OrthoZoom, 1.0f);
			float height = max(_screenHeight / cameraComp.OrthoZoom, 1.0f);
			XMMATRIX projMatrix = XMMatrixOrthographicLH(width, height, 0.0f, 100.0f);

			finalMatrix = XMMatrixTranspose(viewMatrix * projMatrix);
		}
		else
		{
			Vector3 vAt = transComp.GetForward() + transComp.Pos;

			auto vectEye = XMVectorSet(transComp.Pos.X, transComp.Pos.Y, transComp.Pos.Z, 0.0f);
			auto vectAt = XMVectorSet(vAt.X, vAt.Y, vAt.Z, 0.0f);
			auto vectUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			XMMATRIX viewMatrix = XMMatrixLookAtLH(vectEye, vectAt, vectUp);

			XMMATRIX projMatrix = XMMatrixPerspectiveFovLH(
				cameraComp.PerspectiveAngle,
				static_cast<float>(_screenWidth) / static_cast<float>(_screenHeight),
				0.01f,
				1000.0f
			);

			finalMatrix = XMMatrixTranspose(viewMatrix * projMatrix);
		}

		_context->UpdateSubresource(
			cameraComp.ConstantBuffer.Get(),
			0,
			nullptr,
			&finalMatrix,
			0,
			0);

		cameraComp.ResetIsChanged();
	}

	_context->VSSetConstantBuffers(0, 1, cameraComp.ConstantBuffer.GetAddressOf());
}
