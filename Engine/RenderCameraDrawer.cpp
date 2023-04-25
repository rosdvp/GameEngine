#include "pch.h"
#include "RenderCameraDrawer.h"

#include "RenderSlotsConsts.h"
#include "Logger.h"
#include "RenderCameraComp.h"
#include "TransformComp.h"

using namespace BlahEngine;
using namespace DirectX;

RenderCameraDrawer::~RenderCameraDrawer()
{
	Logger::Debug("render module", "camera drawer is destroyed");
}

void RenderCameraDrawer::Init(entt::registry* ecs, 
                              ID3D11Device* device, ID3D11DeviceContext* context, 
                              int screenWidth, int screenHeight)
{
	_ecs = ecs;
	_device = device;
	_context = context;
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	CreateCameraConstantBuffer();

	Logger::Debug("render module", "camera drawer is initialized");
}

void RenderCameraDrawer::BeginFrame()
{
	auto view = _ecs->view<const TransformComp, RenderCameraComp>();

	for (auto entity : view)
	{
		auto [tf, camera] = view.get<const TransformComp, RenderCameraComp>(entity);
		
		if (tf.IsChanged() || camera.IsChanged())
		{
			UpdateCameraConstantBuffer(tf, camera);
			camera.ResetIsChanged();
		}
	}
}

void RenderCameraDrawer::DrawFrame()
{
	auto view = _ecs->view<RenderCameraComp>();
	auto ent = view.front();
	if (ent == entt::null)
		return;

	auto [camera] = view.get(ent);

	_context->VSSetConstantBuffers(SLOT_CAMERA_CONST_BUFFER, 1, _cameraConstantBuffer.GetAddressOf());
	_context->PSSetConstantBuffers(SLOT_CAMERA_CONST_BUFFER, 1, _cameraConstantBuffer.GetAddressOf());
}

void RenderCameraDrawer::CreateCameraConstantBuffer()
{
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = sizeof(CameraConstantBufferData);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;

	HRESULT result = _device->CreateBuffer(
		&constantBufferDesc,
		nullptr,
		_cameraConstantBuffer.GetAddressOf());
	if (FAILED(result))
		throw std::exception("failed to create camera constant buffer");
}

void RenderCameraDrawer::UpdateCameraConstantBuffer(const TransformComp& tf, const RenderCameraComp& camera)
{
	XMVECTOR vEye = tf.Pos;
	auto vUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX viewProjMatrix;
	if (camera.IsOrthographic)
	{
		auto vAt = XMVectorSet(tf.Pos.X, tf.Pos.Y, 0.0f, 0.0f);
		XMMATRIX viewMatrix = XMMatrixLookAtLH(vEye, vAt, vUp);

		float width = std::max(_screenWidth / camera.OrthoZoom, 1.0f);
		float height = std::max(_screenHeight / camera.OrthoZoom, 1.0f);
		XMMATRIX projMatrix = XMMatrixOrthographicLH(width, height, 0.0f, 100.0f);

		viewProjMatrix = XMMatrixTranspose(viewMatrix * projMatrix);
	}
	else
	{
		XMVECTOR vAt = tf.GetForward() + tf.Pos;

		XMMATRIX viewMatrix = XMMatrixLookAtLH(vEye, vAt, vUp);

		XMMATRIX projMatrix = XMMatrixPerspectiveFovLH(
			camera.PerspectiveAngle,
			static_cast<float>(_screenWidth) / static_cast<float>(_screenHeight),
			0.01f,
			1000.0f
		);

		viewProjMatrix = XMMatrixTranspose(viewMatrix * projMatrix);
	}

	CameraConstantBufferData data =
	{
		viewProjMatrix,
		tf.Pos
	};

	_context->UpdateSubresource(
		_cameraConstantBuffer.Get(),
		0,
		nullptr,
		&data,
		0,
		0);
}
