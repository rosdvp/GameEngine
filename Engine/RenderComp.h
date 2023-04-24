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
		DirectX::XMFLOAT4 Color { 1.0f, 1.0f, 1.0f, 1.0f };
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 TexCoord;
	};

	struct Material
	{
		float Diffuse;
		float Ambient;
		float Specular;
		float SpecularSize;
	};

	int ShaderId = -1;

	int VerticesCount = 0;
	std::unique_ptr<Vertex[]> Vertices = nullptr;
	ComPtr<ID3D11Buffer> VertexBuffer;

	int IndicesCount = 0;
	std::unique_ptr<int[]> Indices = nullptr;
	ComPtr<ID3D11Buffer> IndexBuffer;

	ComPtr<ID3D11Buffer> TransformConstantBuffer;

	Material Mat;
	ComPtr<ID3D11Buffer> MaterialConstantBuffer;
	
	ComPtr<ID3D11ShaderResourceView> Texture;
};
}
