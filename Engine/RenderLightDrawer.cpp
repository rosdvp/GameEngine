#include "pch.h"
#include "RenderLightDrawer.h"

#include "RenderSlotsConsts.h"
#include "Logger.h"
#include "RenderLightComp.h"
#include "TransformComp.h"

using namespace BlahEngine;
using namespace DirectX;

RenderLightDrawer::~RenderLightDrawer()
{
	Logger::Debug("render module", "light drawer is destroyed");
}

void RenderLightDrawer::Init(entt::registry* ecs, ID3D11Device* device, ID3D11DeviceContext* context)
{
	_ecs = ecs;
	_device = device;
	_context = context;

	CreateLightConstantBuffer();

	Logger::Debug("render module", "light drawer is initialized");
}

void RenderLightDrawer::BeginFrame()
{
	auto view = _ecs->view<const TransformComp, RenderLightComp>();
	for (auto ent : view)
	{
		auto [tf, light] = view.get(ent);

		if (tf.IsPosOrRotOrScaleChanged() || light.IsChanged())
		{
			UpdateLightConstantBuffer(tf, light);
			light.ResetIsChanged();
		}
	}
}

void RenderLightDrawer::DrawFrame()
{
	auto view = _ecs->view<RenderLightComp>();

	auto ent = view.front();
	if (ent == entt::null)
		return;

	_context->VSSetConstantBuffers(SLOT_LIGHT_CONST_BUFFER, 1, _lightConstantBuffer.GetAddressOf());
	_context->PSSetConstantBuffers(SLOT_LIGHT_CONST_BUFFER, 1, _lightConstantBuffer.GetAddressOf());
}

void RenderLightDrawer::CreateLightConstantBuffer()
{
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = sizeof(LightConstantBufferData);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;

	HRESULT result = _device->CreateBuffer(
		&constantBufferDesc,
		nullptr,
		_lightConstantBuffer.GetAddressOf()
	);
	if (FAILED(result))
		throw std::exception("failed to create lights constant buffer");
}

void RenderLightDrawer::UpdateLightConstantBuffer(const TransformComp& tf, const RenderLightComp& light)
{
	LightConstantBufferData data =
	{
		light.AmbientIntensity,
		tf.Pos,
		-tf.GetForward(),
		light.Color,
		light.ViewProjMatrixForShadowMap
	};
	
	_context->UpdateSubresource(
		_lightConstantBuffer.Get(),
		0,
		nullptr,
		&data,
		0,
		0);
}
