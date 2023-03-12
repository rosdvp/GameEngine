#include "CubeRender.h"

#include <d3dcompiler.h>

using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

struct MatricesDto
{
	XMMATRIX matrixWorld;
	XMMATRIX matrixView;
	XMMATRIX matrixProjection;
};

CubeRender::CubeRender()
{
	_vertexShader = nullptr;
	_pixelShader = nullptr;
	_vertexLayout = nullptr;
	_vertexBuffer = nullptr;

	_constantBuffer = nullptr;
	_indexBuffer = nullptr;
}


bool CubeRender::Init(HWND hwnd)
{
	HRESULT result = S_OK;

	//--------------------------------------------------
	//creating vertex shader
	ID3DBlob* vsBlob = nullptr;
	result = ShadersHelper::CompileShaderFromFile(L"./Shaders/cube_shader.fx", "VS", "vs_4_0", &vsBlob);
	if (FAILED(result))
		return false;
	
	result = _device->CreateVertexShader(vsBlob->GetBufferPointer(),
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
	result = ShadersHelper::CompileShaderFromFile(L"./Shaders/cube_shader.fx", "PS", "ps_4_0", &psBlob);
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

	SimpleVertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) }
	};

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(SimpleVertex) * std::size(vertices);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexInitData = {};
	vertexInitData.pSysMem = vertices;

	result = _device->CreateBuffer(&vertexBufferDesc, &vertexInitData, &_vertexBuffer);
	if (FAILED(result))
		return false;
	
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	_context->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	

	unsigned short indices[] =
	{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned short) * std::size(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA indexInitData = {};
	indexInitData.pSysMem = indices;

	result = _device->CreateBuffer(&indexBufferDesc, &indexInitData, &_indexBuffer);
	if (FAILED(result))
		return false;

	_context->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//--------------------------------------------------
	// creating constant buffer to transfer matrices into shader

	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = sizeof(MatricesDto);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;
	result = _device->CreateBuffer(&constantBufferDesc, nullptr, &_constantBuffer);
	if (FAILED(result))
		return false;

	//--------------------------------------------------
	// creating matrices
	_matrixWorld = XMMatrixIdentity();

	XMVECTOR vectEye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
	XMVECTOR vectAt = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR vectUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	_matrixView = XMMatrixLookAtLH(vectEye, vectAt, vectUp);

	float width = 640.0f;
	float height = 480.0f;
	_matrixProjection = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / height, 0.01f, 100.0f);


	Logger::Debug("render is initialized");
	return true;
}

bool CubeRender::Draw()
{
	static unsigned long dwTimeStart = 0;
	unsigned long dwTimeCur = GetTickCount();
	if (dwTimeStart == 0)
		dwTimeStart = dwTimeCur;
	float time = (dwTimeCur - dwTimeStart) / 1000.0f;

	_matrixWorld = XMMatrixRotationY(time);
	
	MatricesDto dto = {};
	dto.matrixWorld = XMMatrixTranspose(_matrixWorld);
	dto.matrixView = XMMatrixTranspose(_matrixView);
	dto.matrixProjection = XMMatrixTranspose(_matrixProjection);
	_context->UpdateSubresource(_constantBuffer, 0, nullptr, &dto, 0, 0);

	_context->VSSetShader(_vertexShader, nullptr, 0);
	_context->VSSetConstantBuffers(0, 1, &_constantBuffer);
	_context->PSSetShader(_pixelShader, nullptr, 0);
	_context->DrawIndexed(36, 0, 0);

	return true;
}

void CubeRender::Close()
{
	_RELEASE(_constantBuffer);
	_RELEASE(_vertexBuffer);
	_RELEASE(_indexBuffer);
	_RELEASE(_vertexLayout);
	_RELEASE(_vertexShader);
	_RELEASE(_pixelShader);
}