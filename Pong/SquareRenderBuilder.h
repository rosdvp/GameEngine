#pragma once
#include "BlahEngine.h"

class SquareRenderBuilder
{
public:
	static void Build(BlahEngine::RenderComp& renderComp)
	{
		renderComp.VerticesCount = 4;
		renderComp.Vertices = new BlahEngine::RenderComp::Vertex[renderComp.VerticesCount]
		{
		{DirectX::XMFLOAT3(-1.0f, 1.0f, 0.5f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{DirectX::XMFLOAT3(1.0f, 1.0f, 0.5f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{DirectX::XMFLOAT3(1.0f, -1.0f, 0.5f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{DirectX::XMFLOAT3(-1.0f, -1.0f, 0.5f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		};
		renderComp.IndicesCount = 6;
		renderComp.Indices = new int[renderComp.IndicesCount]
		{
			0, 3, 1,
			1, 3, 2
		};
		renderComp.ShaderId = 0;
		renderComp.DrawerId = 0;
	}
};
