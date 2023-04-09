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
			D3D11_BUFFER_DESC constantBufferDesc = {};
			constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			constantBufferDesc.ByteWidth = sizeof(XMMATRIX);
			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferDesc.CPUAccessFlags = 0;

			HRESULT result = _device->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				camera.ViewAndProjConstantBuffer.GetAddressOf());
			if (FAILED(result))
				throw std::exception("failed to create camera constant buffer");

			shouldUpdateBuffer = true;
		}

		if (shouldUpdateBuffer || tf.IsPosOrRotOrScaleChanged() || camera.IsChanged())
		{
			XMMATRIX finalMatrix;
			if (camera.IsOrthographic)
			{
				auto vectEye = tf.Pos.ToXMVector();
				auto vectAt = XMVectorSet(tf.Pos.X, tf.Pos.Y, 0.0f, 0.0f);
				auto vectUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
				XMMATRIX viewMatrix = XMMatrixLookAtLH(vectEye, vectAt, vectUp);

				float width = std::max(_screenWidth / camera.OrthoZoom, 1.0f);
				float height = std::max(_screenHeight / camera.OrthoZoom, 1.0f);
				XMMATRIX projMatrix = XMMatrixOrthographicLH(width, height, 0.0f, 100.0f);

				finalMatrix = XMMatrixTranspose(viewMatrix * projMatrix);
			}
			else
			{
				auto vectEye = tf.Pos.ToXMVector();
				auto vectUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
				XMVECTOR vectAt = (tf.GetForward() + tf.Pos).ToXMVector();

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
				camera.ViewAndProjConstantBuffer.Get(),
				0,
				nullptr,
				&finalMatrix,
				0,
				0);

			camera.ResetIsChanged();
		}

		_context->VSSetConstantBuffers(0, 1, camera.ViewAndProjConstantBuffer.GetAddressOf());
	}
}
