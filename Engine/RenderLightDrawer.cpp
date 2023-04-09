#include "pch.h"
#include "RenderLightDrawer.h"

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

	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = sizeof(LightBufferData);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;

	HRESULT result = _device->CreateBuffer(
		&constantBufferDesc,
		nullptr,
		_lightConstantBuffer.GetAddressOf());
	if (FAILED(result))
		throw std::exception("failed to create lights constant buffer");

	Logger::Debug("render module", "light drawer is initialized");
}

void RenderLightDrawer::Draw()
{
	auto view = _ecs->view<const TransformComp, RenderLightComp>();
	for (auto ent : view)
	{
		auto& tf = view.get<const TransformComp>(ent);
		auto& light = view.get<RenderLightComp>(ent);

		bool shouldUpdateBuffer = false;

		if (light.IsDirty)
		{
			light.IsDirty = false;

			_lightBufferData.LightColor = light.Color;

			shouldUpdateBuffer = true;
		}
		if (tf.IsPosOrRotOrScaleChanged())
		{
			_lightBufferData.LightDir = tf.GetForward().ToXMFloat4();

			shouldUpdateBuffer = true;
		}

		if (shouldUpdateBuffer)
		{
			_context->UpdateSubresource(
				_lightConstantBuffer.Get(),
				0,
				nullptr,
				&_lightBufferData,
				0,
				0);
		}

		_context->PSSetConstantBuffers(2, 1, _lightConstantBuffer.GetAddressOf());
	}
}