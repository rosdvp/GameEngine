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
	_isInited(false)
{
	_renderBackgroundDefault = std::make_unique<RenderBackground>();
	_renderShaders.push_back(std::make_unique<RenderShader>());
	_renderDrawers.push_back(std::make_unique<RenderDrawer>());
}

RenderModule::~RenderModule() noexcept(false)
{								   
	Logger::Debug("render module", "destroyed");
}


void RenderModule::Init(entt::registry* ecs, const TimeModule* timeModule,
	HWND hwnd, int screenWidth, int screenHeight)
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

	for (auto& s : _renderShaders)
		s->Init(_device.Get());

	//--------------------------------------------------
	//initializing default background
	_renderBackgroundDefault->Init(_device.Get(), _timeModule);

	//--------------------------------------------------

	_isInited = true;
	Logger::Debug("render module", "initialized");
}

IDXGISwapChain* RenderModule::GetSwapChain() const
{
	return _swapChain.Get();
}


void RenderModule::SetRenderBackground(RenderBackground&& background)
{
	if (!_isInited)
		throw std::exception("render module is not initialized");

	background.Init(_device.Get(), _timeModule);
	_renderBackgroundCustom = std::make_unique<RenderBackground>(background);
}

void RenderModule::AddRenderShader(RenderShader&& shader)
{
	if (!_isInited)
		throw std::exception("render module is not initialized");

	shader.Init(_device.Get());
	_renderShaders.push_back(std::make_unique<RenderShader>(shader));
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

	DrawCamera();
	
	auto view = _ecs->view<const TransformComp, RenderComp>();
	for (auto ent : view)
	{
		auto [tf, render] = view.get<const TransformComp, RenderComp>(ent);

		_renderShaders[render.ShaderId]->ApplyToContext(_context.Get());
		_renderDrawers[render.DrawerId]->Draw(_device.Get(), _context.Get(), tf, render);
	}
}

void RenderModule::EndDrawFrame()
{
	_context->OMSetRenderTargets(0, nullptr, nullptr);
	_swapChain->Present(1, 0);
}



void RenderModule::DrawCamera()
{
	auto view = _ecs->view<const TransformComp, RenderCameraComp>();

	for (auto entity : view)
	{
		auto [tf, camera] = view.get<const TransformComp, RenderCameraComp>(entity);

		bool shouldUpdateBuffer = false;
		if (camera.ConstantBuffer.Get() == nullptr)
		{
			D3D11_BUFFER_DESC constantBufferDesc = {};
			constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			constantBufferDesc.ByteWidth = sizeof(XMMATRIX);
			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferDesc.CPUAccessFlags = 0;

			HRESULT result = _device->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				camera.ConstantBuffer.GetAddressOf());
			if (FAILED(result))
				throw std::exception("failed to create camera constant buffer");

			shouldUpdateBuffer = true;
		}

		if (shouldUpdateBuffer || tf.IsPosOrRotOrScaleChanged() || camera.IsChanged())
		{
			XMMATRIX finalMatrix;
			if (camera.IsOrthographic)
			{
				auto vectEye = XMVectorSet(tf.Pos.X, tf.Pos.Y, tf.Pos.Z, 0.0f);
				auto vectAt = XMVectorSet(tf.Pos.X, tf.Pos.Y, 0.0f, 0.0f);
				auto vectUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
				XMMATRIX viewMatrix = XMMatrixLookAtLH(vectEye, vectAt, vectUp);

				float width         = std::max(_screenWidth / camera.OrthoZoom, 1.0f);
				float height        = std::max(_screenHeight / camera.OrthoZoom, 1.0f);
				XMMATRIX projMatrix = XMMatrixOrthographicLH(width, height, 0.0f, 100.0f);

				finalMatrix = XMMatrixTranspose(viewMatrix * projMatrix);
			}
			else
			{
				Vector3 vAt = tf.GetForward() + tf.Pos;

				auto vectEye = XMVectorSet(tf.Pos.X, tf.Pos.Y, tf.Pos.Z, 0.0f);
				auto vectAt = XMVectorSet(vAt.X, vAt.Y, vAt.Z, 0.0f);
				auto vectUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
				XMMATRIX viewMatrix = XMMatrixLookAtLH(vectEye, vectAt, vectUp);

				XMMATRIX projMatrix = XMMatrixPerspectiveFovLH(
					camera.PerspectiveAngle,
					static_cast<float>(_screenWidth) / static_cast<float>(_screenHeight),
					0.01f,
					1000.0f
				);

				finalMatrix = XMMatrixTranspose(viewMatrix * projMatrix);
			}

			_context->UpdateSubresource(
				camera.ConstantBuffer.Get(),
				0,
				nullptr,
				&finalMatrix,
				0,
				0);

			camera.ResetIsChanged();
		}

		_context->VSSetConstantBuffers(0, 1, camera.ConstantBuffer.GetAddressOf());
	}
}
