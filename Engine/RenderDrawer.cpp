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

void RenderDrawer::DrawEntity(ID3D11Device* device, ID3D11DeviceContext* context, 
                        const TransformComp& transComp, RenderComp& renderComp)
{
	bool shouldUpdateMatrix = false;

	if (renderComp.VertexBuffer.Get() == nullptr)
	{
		HRESULT result = S_OK;
		
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(RenderComp::Vertex) * renderComp.VerticesCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexInitData = {};
		vertexInitData.pSysMem = renderComp.Vertices;

		result = device->CreateBuffer(&vertexBufferDesc, &vertexInitData, renderComp.VertexBuffer.GetAddressOf());
		if (FAILED(result))
			throw std::exception("failed to create vertex buffer");

		//---------------------------------------------

		D3D11_BUFFER_DESC indexBufferDesc = {};
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(int) * renderComp.IndicesCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA indexInitData = {};
		indexInitData.pSysMem = renderComp.Indices;

		result = device->CreateBuffer(&indexBufferDesc, &indexInitData, renderComp.IndexBuffer.GetAddressOf());
		if (FAILED(result))
			throw std::exception("failed to create index buffer");

		//---------------------------------------------

		D3D11_BUFFER_DESC constantBufferDesc = {};
		constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		constantBufferDesc.ByteWidth = sizeof(XMMATRIX);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = 0;

		result = device->CreateBuffer(
			&constantBufferDesc,
			nullptr,
			renderComp.MatrixConstantBuffer.GetAddressOf());
		if (FAILED(result))
			throw std::exception("failed to create matrix constant buffer");

		shouldUpdateMatrix = true;
	}

	if (shouldUpdateMatrix || transComp.IsChanged())
	{
		auto matrix =
			XMMatrixTranspose(XMMatrixTranslation(transComp.Pos.X, transComp.Pos.Y, transComp.Pos.Z)) *
			XMMatrixTranspose(XMMatrixScaling(transComp.Scale.X, transComp.Scale.Y, transComp.Scale.Z));
		context->UpdateSubresource(
			renderComp.MatrixConstantBuffer.Get(),
			0,
			nullptr,
			&matrix,
			0,
			0);
	}
	context->VSSetConstantBuffers(1, 1, renderComp.MatrixConstantBuffer.GetAddressOf());

	UINT stride = sizeof(RenderComp::Vertex);
	UINT offset = 0;
	
	context->IASetVertexBuffers(
		0,
		1,
		 renderComp.VertexBuffer.GetAddressOf(),
		&stride,
		&offset);

	context->IASetIndexBuffer(renderComp.IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->DrawIndexed(renderComp.IndicesCount, 0, 0);
}
