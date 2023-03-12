#include "TestRender.h"

#include <d3dcompiler.h>

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

TestRender::TestRender()
{
	_vertexShader = nullptr;
	_pixelShader = nullptr;
	_vertexLayout = nullptr;
	_vertexBuffer = nullptr;
	_indexBuffer = nullptr;
}


bool TestRender::Init(HWND hwnd)
{
	HRESULT result = S_OK;

	//--------------------------------------------------
	//creating vertex shader
	ID3DBlob* vsBlob = nullptr;
	result = ShadersHelper::CompileShaderFromFile(
		L"./Shaders/triangle_shader.fx",
		"VS",
		"vs_5_0",
		&vsBlob);
	if (FAILED(result))
		return false;

	result = _device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		nullptr,
		&_vertexShader);
	if (FAILED(result))
	{
		Logger::Error("failed to create vertex shader");
		_RELEASE(vsBlob);
		return false;
	}

	Logger::Debug("vertex shader created");

	//--------------------------------------------------
	//providing layout for vertex shader
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			12,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	result = _device->CreateInputLayout(
		layoutDesc,
		ARRAYSIZE(layoutDesc),
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&_vertexLayout);

	_RELEASE(vsBlob);
	if (FAILED(result))
		return false;

	_context->IASetInputLayout(_vertexLayout);

	Logger::Debug("vertex shader connected");

	//--------------------------------------------------
	//creating pixel shader
	ID3DBlob* psBlob = nullptr;
	result = ShadersHelper::CompileShaderFromFile(
		L"./Shaders/triangle_shader.fx",
		"PS",
		"ps_5_0",
		&psBlob);
	if (FAILED(result))
		return false;

	result = _device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		nullptr,
		&_pixelShader);

	_RELEASE(psBlob);
	if (FAILED(result))
	{
		Logger::Error("failed to compile pixel shader");
		_RELEASE(psBlob);
		return false;
	}

	Logger::Debug("pixel shader created");

	//--------------------------------------------------
	// creating a figure

	Vertex vertices[] =
	{
		{ XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		{XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)}
	};

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * std::size(vertices);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexInitData = {};
	vertexInitData.pSysMem = vertices;

	result = _device->CreateBuffer(&vertexBufferDesc, &vertexInitData, &_vertexBuffer);
	if (FAILED(result))
		return false;

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	_context->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);


	int indices[] =
	{
		0, 1, 2
	};

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(int) * std::size(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexInitData = {};
	indexInitData.pSysMem = indices;

	result = _device->CreateBuffer(&indexBufferDesc, &indexInitData, &_indexBuffer);
	if (FAILED(result))
		return false;

	_context->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//--------------------------------------------------
	// creating constant buffer to transfer matrices into shader

	Logger::Debug("render is initialized");
	return true;
}

bool TestRender::Draw()
{
	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(640);
	viewport.Height = static_cast<float>(480);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;
	_context->RSSetViewports(1, &viewport);

	_context->IASetInputLayout(_vertexLayout);
	_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_context->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	_context->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

	_context->VSSetShader(_vertexShader, nullptr, 0);
	_context->PSSetShader(_pixelShader, nullptr, 0);

	_context->DrawIndexed(3, 0, 0);

	return true;
}

void TestRender::Close()
{
	_RELEASE(_vertexBuffer);
	_RELEASE(_indexBuffer);
	_RELEASE(_vertexLayout);
	_RELEASE(_vertexShader);
	_RELEASE(_pixelShader);
}