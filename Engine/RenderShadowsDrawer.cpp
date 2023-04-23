#include "pch.h"
#include "RenderShadowsDrawer.h"

#include "Logger.h"
#include "RenderLightComp.h"
#include "TransformComp.h"

#include "DirectXColors.h"
#include "ERenderSlot.h"
#include "RenderComp.h"

using namespace BlahEngine;
using namespace DirectX;

RenderShadowsDrawer::~RenderShadowsDrawer()
{
	Logger::Debug("render", "shadows drawer is destroyed");
}

void RenderShadowsDrawer::Init(entt::registry* ecs, ID3D11Device* device, ID3D11DeviceContext* context)
{
	_ecs = ecs;
	_device = device;
	_context = context;

	HRESULT result = S_OK;

	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.DepthClipEnable = true;

	result = _device->CreateRasterizerState(&rasterizerDesc, _shadowRasterizer.GetAddressOf());
	if (FAILED(result))
		throw std::exception("failed to create shadows rasterizer");


	_shadowViewport = {};
	_shadowViewport.Width = SHADOWMAP_WIDTH;
	_shadowViewport.Height = SHADOWMAP_HEIGHT;
	_shadowViewport.MinDepth = 0.0f;
	_shadowViewport.MaxDepth = 1.0f;


	CreateShadowMap();

	_shader.Init(device);

	Logger::Debug("render", "shadows drawer is initialized");
}

void RenderShadowsDrawer::BeginFrame()
{
	auto view = _ecs->view<const TransformComp, RenderLightComp>();

	auto ent = view.front();
	if (ent == entt::null)
		return;
	auto [tf, light] = view.get(ent);

	bool shouldUpdateBuffer = false;
	if (_shadowConstantBuffer.Get() == nullptr)
	{
		CreateShadowConstantBuffer();
		shouldUpdateBuffer = true;
	}
	if (shouldUpdateBuffer || tf.IsPosOrRotOrScaleChanged() || light.IsChanged())
	{
		XMVECTOR vEye = tf.Pos;
		XMVECTOR vUp{ 0.0f, 1.0f, 0.0f, 0.0f };
		XMVECTOR vAt = tf.GetForward() + tf.Pos;

		XMMATRIX viewMatrix = XMMatrixLookAtLH(vEye, vAt, vUp);
		XMMATRIX projMatrix = XMMatrixOrthographicLH(
			SHADOWMAP_WIDTH * 0.01f,
			SHADOWMAP_HEIGHT * 0.01f,
			0.0f,
			100.0f
		);
		//XMMATRIX projMatrix = XMMatrixPerspectiveFovLH(
		//	static_cast<float>(XM_PI) / 4.0f,
		//	static_cast<float>(SHADOWMAP_WIDTH) / static_cast<float>(SHADOWMAP_HEIGHT),
		//	1.0f,
		//	100.0f
		//);
		light.ViewProjMatrixForShadowMap = XMMatrixTranspose(viewMatrix * projMatrix);

		UpdateShadowConstantBuffer(light.ViewProjMatrixForShadowMap);
	}
}

void RenderShadowsDrawer::DrawFrame(ID3D11RenderTargetView* renderTarget)
{
	_context->ClearState();

	_context->ClearDepthStencilView(_shadowDepthView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (renderTarget == nullptr)
	{
		_context->OMSetRenderTargets(
			0,
			nullptr,
			_shadowDepthView.Get()
		);
	}
	else
	{
		_context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
		_context->OMSetRenderTargets(
			1,
			&renderTarget,
			_shadowDepthView.Get()
		);
	}

	_context->RSSetState(_shadowRasterizer.Get());
	_context->RSSetViewports(1, &_shadowViewport);

	_context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	_shader.ApplyToContext(_context);

	_context->VSSetConstantBuffers(SLOT_SHADOW_CONST_BUFFER, 1, _shadowConstantBuffer.GetAddressOf());

	auto view = _ecs->view<const TransformComp, const RenderComp>(entt::exclude<RenderLightComp>);
	for (auto ent : view)
	{
		auto [tf, render] = view.get(ent);

		UINT stride = sizeof(RenderComp::Vertex);
		UINT offset = 0;

		_context->IASetVertexBuffers(
			0,
			1,
			render.VertexBuffer.GetAddressOf(),
			&stride,
			&offset
		);
		_context->IASetIndexBuffer(
			render.IndexBuffer.Get(),
			DXGI_FORMAT_R32_UINT, 0);

		_context->VSSetConstantBuffers(
			SLOT_TRANSFORM_CONST_BUFFER,
			1,
			render.TransformConstantBuffer.GetAddressOf()
		);

		_context->DrawIndexed(render.IndicesCount, 0, 0);
	}
}

void RenderShadowsDrawer::ApplyShadowMapToContext()
{
	_context->PSSetShaderResources(SLOT_SHADOW_MAP, 1, _shadowResView.GetAddressOf());
}

void RenderShadowsDrawer::CreateShadowMap()
{
	HRESULT result = S_OK;

	D3D11_TEXTURE2D_DESC shadowMapDesc {};
	shadowMapDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	shadowMapDesc.MipLevels = 1;
	shadowMapDesc.ArraySize = 1;
	shadowMapDesc.SampleDesc.Count = 1;
	shadowMapDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	shadowMapDesc.Width = SHADOWMAP_WIDTH;
	shadowMapDesc.Height = SHADOWMAP_HEIGHT;

	result = _device->CreateTexture2D(
		&shadowMapDesc,
		nullptr,
		_shadowMap.GetAddressOf()
	);
	if (FAILED(result))
		throw std::exception("failed to create shadow map");


	D3D11_DEPTH_STENCIL_VIEW_DESC shadowDepthViewDesc {};
	shadowDepthViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	shadowDepthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	shadowDepthViewDesc.Texture2D.MipSlice = 0;

	result = _device->CreateDepthStencilView(
		_shadowMap.Get(),
		&shadowDepthViewDesc,
		_shadowDepthView.GetAddressOf()
	);
	if (FAILED(result))
		throw std::exception("failed to create shadow map depth view");


	D3D11_SHADER_RESOURCE_VIEW_DESC shadowResViewDesc {};
	shadowResViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shadowResViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shadowResViewDesc.Texture2D.MipLevels = 1;

	result = _device->CreateShaderResourceView(
		_shadowMap.Get(),
		&shadowResViewDesc,
		_shadowResView.GetAddressOf()
	);
	if (FAILED(result))
		throw std::exception("failed to create shadow map shader resource view");
}

void RenderShadowsDrawer::CreateShadowConstantBuffer()
{
	D3D11_BUFFER_DESC shadowsConstantBufferDesc = {};
	shadowsConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	shadowsConstantBufferDesc.ByteWidth = sizeof(ShadowConstantBufferData);
	shadowsConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	shadowsConstantBufferDesc.CPUAccessFlags = 0;

	HRESULT result = _device->CreateBuffer(
		&shadowsConstantBufferDesc,
		nullptr,
		_shadowConstantBuffer.GetAddressOf());
	if (FAILED(result))
		throw std::exception("failed to create shadows constant buffer");
}

void RenderShadowsDrawer::UpdateShadowConstantBuffer(XMMATRIX viewProjMatrix)
{
	ShadowConstantBufferData data
	{
		viewProjMatrix
	};
	_context->UpdateSubresource(
		_shadowConstantBuffer.Get(),
		0,
		nullptr,
		&data,
		0,
		0
	);
}
