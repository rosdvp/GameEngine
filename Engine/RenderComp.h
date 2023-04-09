#pragma once
#include "pch.h"
#include <directxmath.h>

namespace BlahEngine
{
struct RenderComp
{
	struct Vertex
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT4 Color;
		DirectX::XMFLOAT2 TexCoord;
	};

	int ShaderId = -1;
	int DrawerId = -1;

	int VerticesCount = 0;
	std::unique_ptr<Vertex[]> Vertices = nullptr;
	ComPtr<ID3D11Buffer> VertexBuffer;

	int IndicesCount = 0;
	std::unique_ptr<int[]> Indices = nullptr;
	ComPtr<ID3D11Buffer> IndexBuffer;

	ComPtr<ID3D11Buffer> MatrixConstantBuffer;
	
	ComPtr<ID3D11ShaderResourceView> Texture;
};
}
