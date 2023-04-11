#include "pch.h"
#include "RenderCameraDrawer.h"

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

	Logger::Debug("render module", "camera drawer is initialized");
}

void RenderCameraDrawer::Draw()
{
	auto view = _ecs->view<const TransformComp, RenderCameraComp>();

	for (auto entity : view)
	{
		auto [tf, camera] = view.get<const TransformComp, RenderCameraComp>(entity);

		bool shouldUpdateBuffer = false;
		if (camera.ViewAndProjConstantBuffer.Get() == nullptr)
		{
			CreateCameraConstantBuffer(camera);
			shouldUpdateBuffer = true;
		}

		if (shouldUpdateBuffer || tf.IsPosOrRotOrScaleChanged() || camera.IsChanged())
		{
			UpdateCameraConstantBuffer(tf, camera);
			camera.ResetIsChanged();
		}

		_context->VSSetConstantBuffers(0, 1, camera.ViewAndProjConstantBuffer.GetAddressOf());
		_context->PSSetConstantBuffers(0, 1, camera.ViewAndProjConstantBuffer.GetAddressOf());
	}
}

void RenderCameraDrawer::CreateCameraConstantBuffer(RenderCameraComp& camera)
{
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = sizeof(CameraConstantBufferData);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;

	HRESULT result = _device->CreateBuffer(
		&constantBufferDesc,
		nullptr,
		camera.ViewAndProjConstantBuffer.GetAddressOf());
	if (FAILED(result))
		throw std::exception("failed to create camera constant buffer");
}

void RenderCameraDrawer::UpdateCameraConstantBuffer(const TransformComp& tf, RenderCameraComp& camera)
{
	XMVECTOR vEye = tf.Pos;
	auto vUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX cameraMatrix;
	if (camera.IsOrthographic)
	{
		auto vAt = XMVectorSet(tf.Pos.X, tf.Pos.Y, 0.0f, 0.0f);
		XMMATRIX viewMatrix = XMMatrixLookAtLH(vEye, vAt, vUp);

		float width = std::max(_screenWidth / camera.OrthoZoom, 1.0f);
		float height = std::max(_screenHeight / camera.OrthoZoom, 1.0f);
		XMMATRIX projMatrix = XMMatrixOrthographicLH(width, height, 0.0f, 100.0f);

		cameraMatrix = XMMatrixTranspose(viewMatrix * projMatrix);
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

		cameraMatrix = XMMatrixTranspose(viewMatrix * projMatrix);
	}

	CameraConstantBufferData data =
	{
		cameraMatrix,
		tf.Pos
	};

	_context->UpdateSubresource(
		camera.ViewAndProjConstantBuffer.Get(),
		0,
		nullptr,
		&data,
		0,
		0);
}
