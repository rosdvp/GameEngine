#include "pch.h"
#include "RenderDrawer.h"

#include "RenderComp.h"
#include "TransformComp.h"

using namespace BlahEngine;
using namespace DirectX;

RenderDrawer::RenderDrawer()
{
	
}

RenderDrawer::~RenderDrawer()
{
	
}

void RenderDrawer::Draw(ID3D11Device* device, ID3D11DeviceContext* context, 
                        const TransformComp& tf, RenderComp& render)
{
	bool shouldUpdateMatrix = false;

	if (render.VertexBuffer.Get() == nullptr)
	{
		HRESULT result = S_OK;
		
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(RenderComp::Vertex) * render.VerticesCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexInitData = {};
		vertexInitData.pSysMem = render.Vertices.get();

		result = device->CreateBuffer(&vertexBufferDesc, &vertexInitData, render.VertexBuffer.GetAddressOf());
		if (FAILED(result))
			throw std::exception("failed to create vertex buffer");

		//---------------------------------------------

		D3D11_BUFFER_DESC indexBufferDesc = {};
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(int) * render.IndicesCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA indexInitData = {};
		indexInitData.pSysMem = render.Indices.get();

		result = device->CreateBuffer(&indexBufferDesc, &indexInitData, render.IndexBuffer.GetAddressOf());
		if (FAILED(result))
			throw std::exception("failed to create index buffer");

		//---------------------------------------------

		D3D11_BUFFER_DESC constantBufferDesc = {};
		constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		constantBufferDesc.ByteWidth = sizeof(TransformConstantBufferData);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = 0;

		result = device->CreateBuffer(
			&constantBufferDesc,
			nullptr,
			render.TransformConstantBuffer.GetAddressOf());
		if (FAILED(result))
			throw std::exception("failed to create matrix constant buffer");

		shouldUpdateMatrix = true;
	}

	if (shouldUpdateMatrix || tf.IsPosOrRotOrScaleChanged())
	{
		auto worldMatrix =
			XMMatrixScaling(tf.Scale.X, tf.Scale.Y, tf.Scale.Z) *
			//XMMatrixRotationRollPitchYaw(tf.Rot.X, tf.Rot.Y, tf.Rot.Z) *
			//XMMatrixRotationRollPitchYaw(rot.X, rot.Y, rot.Z) *
			XMMatrixRotationQuaternion(tf.Rot.GetQuaternion()) *
			XMMatrixTranslation(tf.GlobalPos.X, tf.GlobalPos.Y, tf.GlobalPos.Z);
		
		TransformConstantBufferData data =
		{
			XMMatrixTranspose(worldMatrix)
		};

		context->UpdateSubresource(
			render.TransformConstantBuffer.Get(),
			0,
			nullptr,
			&data,
			0,
			0);
	}
	context->VSSetConstantBuffers(1, 1, render.TransformConstantBuffer.GetAddressOf());
	
	context->PSSetShaderResources(0, 1, render.Texture.GetAddressOf());
		
	UINT stride = sizeof(RenderComp::Vertex);
	UINT offset = 0;
	
	context->IASetVertexBuffers(
		0,
		1,
		 render.VertexBuffer.GetAddressOf(),
		&stride,
		&offset);

	context->IASetIndexBuffer(render.IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->DrawIndexed(render.IndicesCount, 0, 0);
}
