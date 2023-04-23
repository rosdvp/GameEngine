#include "pch.h"
#include "RenderShaderSimpleLit.h"

using namespace BlahEngine;

void RenderShaderSimpleLit::Init(ID3D11Device* device)
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
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	CreateVertexShader(L"./Shaders/simple_lit_shader.hlsl",
		"VS",
		"vs_5_0",
		device,
		layoutDesc,
		sizeof(layoutDesc) / sizeof(D3D11_INPUT_ELEMENT_DESC));

	CreatePixelShader(
		L"./Shaders/simple_lit_shader.hlsl",
		"PS",
		"ps_5_0",
		device);

	CreateMainTexSampler(device);
	CreateShadowMapSampler(device);
}