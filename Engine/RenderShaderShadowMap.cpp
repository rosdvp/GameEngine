#include "pch.h"
#include "RenderShaderShadowMap.h"

using namespace BlahEngine;

void RenderShaderShadowMap::Init(ID3D11Device* device)
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
		}
	};

	CreateVertexShader(
		L"./Shaders/shadow_map_shader.hlsl",
		"VS",
		"vs_5_0",
		device,
		layoutDesc,
		sizeof(layoutDesc) / sizeof(D3D11_INPUT_ELEMENT_DESC)
	);
	//CreatePixelShader(
	//	L"./Shaders/shadow_map_shader.hlsl",
	//	"PS",
	//	"ps_5_0",
	//	device
	//);
}