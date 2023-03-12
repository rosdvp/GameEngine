/*#include "SquareRenderObject.h"

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};


SquareRenderObject::SquareRenderObject(int shaderIndex) :
	RenderComp(shaderIndex)
{ }

void SquareRenderObject::InitImpl(ID3D11Device* device)
{
	HRESULT result = S_OK;

	Vertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, 0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
	};

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(vertices) * std::size(vertices);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexInitData = {};
	vertexInitData.pSysMem = vertices;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexInitData, &_vertexBuffer);
	if (FAILED(result))
		throw std::exception("failed to create vertex buffer");


	int indices[] =
	{
		0, 3, 1,
		1, 3, 2
	};

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(int) * std::size(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA indexInitData = {};
	indexInitData.pSysMem = indices;

	result = device->CreateBuffer(&indexBufferDesc, &indexInitData, &_indexBuffer);
	if (FAILED(result))
		throw std::exception("failed to create index buffer");


	Logger::Debug("render objects", "triangle initialized");
}

void SquareRenderObject::DrawFrameImpl(const RenderFrameDto& dto)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	dto.Context->IASetVertexBuffers(
		0,
		1,
		_vertexBuffer.GetAddressOf(),
		&stride,
		&offset);

	dto.Context->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	dto.Context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dto.Context->DrawIndexed(6, 0, 0);

}*/