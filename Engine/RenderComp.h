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
	};

	int ShaderId = -1;
	int DrawerId = -1;

	Vertex* Vertices = nullptr;
	int VerticesCount = 0;

	int* Indices = nullptr;
	int IndicesCount = 0;
	
	ComPtr<ID3D11Buffer> VertexBuffer;
	ComPtr<ID3D11Buffer> IndexBuffer;
	ComPtr<ID3D11Buffer> MatrixConstantBuffer;

	void Release()
	{
		delete Vertices;
		Vertices = nullptr;
		delete Indices;
		Indices = nullptr;
		VertexBuffer.Reset();
		IndexBuffer.Reset();
		MatrixConstantBuffer.Reset();
	}
};
}
