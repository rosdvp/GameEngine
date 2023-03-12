#include "SimpleRenderShader.h"

void SimpleRenderShader::Init(ID3D11Device* device)
{
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

	CreateVertexShader(L"./Shaders/test_shader.hlsl",
		"VSMain",
		"vs_5_0",
		device,
		layoutDesc,
		2);

	CreatePixelShader(
		L"./Shaders/test_shader.hlsl",
		"PSMain",
		"ps_5_0",
		device);
}