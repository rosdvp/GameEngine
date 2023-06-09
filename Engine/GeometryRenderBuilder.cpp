#include "pch.h"
#include "GeometryRenderBuilder.h"

using namespace BlahEngine;
using namespace DirectX;

void GeometryRenderBuilder::BuildSquare(RenderComp& render, const Color& color)
{
	RenderComp::Vertex vertices[] =
	{
		{XMFLOAT3(-1.0f, -1.0f, 0.5f), color},
		{XMFLOAT3(-1.0f, 1.0f, 0.5f), color},
		{XMFLOAT3(1.0f, 1.0f, 0.5f), color},
		{XMFLOAT3(1.0f, -1.0f, 0.5f), color},
	};
	int indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};
	FillRender(render, vertices, std::size(vertices), indices, std::size(indices));
}


void GeometryRenderBuilder::BuildCube(RenderComp& render, const Color& color)
{
	RenderComp::Vertex vertices[] =
	{
		{XMFLOAT3(-0.5f, 0.5f, -0.5f), color, XMFLOAT3(0.0f, 1.0f, 0.0f)},
		{XMFLOAT3(0.5f, 0.5f, -0.5f), color, XMFLOAT3(0.0f, 1.0f, 0.0f)},
		{XMFLOAT3(0.5f, 0.5f, 0.5f), color, XMFLOAT3(0.0f, 1.0f, 0.0f)},
		{XMFLOAT3(-0.5f, 0.5f, 0.5f), color, XMFLOAT3(0.0f, 1.0f, 0.0f)},

		{XMFLOAT3(-0.5f, -0.5f, -0.5f), color, XMFLOAT3(0.0f, -1.0f, 0.0f)},
		{XMFLOAT3(0.5f, -0.5f, -0.5f), color, XMFLOAT3(0.0f, -1.0f, 0.0f)},
		{XMFLOAT3(0.5f, -0.5f, 0.5f), color, XMFLOAT3(0.0f, -1.0f, 0.0f)},
		{XMFLOAT3(-0.5f, -0.5f, 0.5f), color, XMFLOAT3(0.0f, -1.0f, 0.0f)},

		{XMFLOAT3(-0.5f, -0.5f, 0.5f), color, XMFLOAT3(-1.0f, 0.0f, 0.0f)},
		{XMFLOAT3(-0.5f, -0.5f, -0.5f), color, XMFLOAT3(-1.0f, 0.0f, 0.0f)},
		{XMFLOAT3(-0.5f, 0.5f, -0.5f), color, XMFLOAT3(-1.0f, 0.0f, 0.0f)},
		{XMFLOAT3(-0.5f, 0.5f, 0.5f), color, XMFLOAT3(-1.0f, 0.0f, 0.0f)},

		{XMFLOAT3(0.5f, -0.5f, 0.5f), color, XMFLOAT3(1.0f, 0.0f, 0.0f)},
		{XMFLOAT3(0.5f, -0.5f, -0.5f), color, XMFLOAT3(1.0f, 0.0f, 0.0f)},
		{XMFLOAT3(0.5f, 0.5f, -0.5f), color, XMFLOAT3(1.0f, 0.0f, 0.0f)},
		{XMFLOAT3(0.5f, 0.5f, 0.5f), color, XMFLOAT3(1.0f, 0.0f, 0.0f)},

		{XMFLOAT3(-0.5f, -0.5f, -0.5f), color, XMFLOAT3(0.0f, 0.0f, -1.0f)},
		{XMFLOAT3(0.5f, -0.5f, -0.5f), color, XMFLOAT3(0.0f, 0.0f, -1.0f)},
		{XMFLOAT3(0.5f, 0.5f, -0.5f), color, XMFLOAT3(0.0f, 0.0f, -1.0f)},
		{XMFLOAT3(-0.5f, 0.5f, -0.5f), color, XMFLOAT3(0.0f, 0.0f, -1.0f)},

		{XMFLOAT3(-0.5f, -0.5f, 0.5f), color, XMFLOAT3(0.0f, 0.0f, 1.0f)},
		{XMFLOAT3(0.5f, -0.5f, 0.5f), color, XMFLOAT3(0.0f, 0.0f, 1.0f)},
		{XMFLOAT3(0.5f, 0.5f, 0.5f), color, XMFLOAT3(0.0f, 0.0f, 1.0f)},
		{XMFLOAT3(-0.5f, 0.5f, 0.5f), color, XMFLOAT3(0.0f, 0.0f, 1.0f)},
	};

	int indices[] =
	{
		3, 1, 0,
		2, 1, 3,

		6, 4, 5,
		7, 4, 6,

		11, 9, 8,
		10, 9, 11,

		14, 12, 13,
		15, 12, 14,

		19, 17, 16,
		18, 17, 19,

		22, 20, 21,
		23, 20, 22
	};

	FillRender(render, vertices, std::size(vertices), indices, std::size(indices));
}


void GeometryRenderBuilder::BuildSphere(RenderComp& render, size_t tessellation, const Color& color)
{
	std::vector<RenderComp::Vertex> vertices;
	std::vector<int> indices;

	if (tessellation < 3)
		throw std::invalid_argument("tessellation parameter must be at least 3");

	const size_t verticalSegments   = tessellation;
	const size_t horizontalSegments = tessellation * 2;

	// Create rings of vertices at progressively higher latitudes.
	for (size_t i = 0; i <= verticalSegments; i++)
	{
		const float latitude = static_cast<float>(i) * XM_PI / static_cast<float>(verticalSegments) - XM_PIDIV2;
		float dy, dxz;

		XMScalarSinCos(&dy, &dxz, latitude);

		// Create a single ring of vertices at this latitude.
		for (size_t j = 0; j <= horizontalSegments; j++)
		{
			const float longitude = static_cast<float>(j) * XM_2PI / static_cast<float>(horizontalSegments);
			float dx, dz;

			XMScalarSinCos(&dx, &dz, longitude);

			dx *= dxz;
			dz *= dxz;

			XMFLOAT3 pos = { dx / 2, dy / 2, dz / 2 };
			XMVECTOR rawNormal = { dx, dy, dz };
			rawNormal = XMVector3Normalize(rawNormal);
			XMFLOAT3 normal;
			XMStoreFloat3(&normal, rawNormal);

			vertices.push_back({ pos, color, normal});
		}
	}

	// Fill the index buffer with triangles joining each pair of latitude rings.
	const size_t stride = horizontalSegments + 1;

	for (size_t i = 0; i < verticalSegments; i++)
	{
		for (size_t j = 0; j <= horizontalSegments; j++)
		{
			const size_t nextI = i + 1;
			const size_t nextJ = (j + 1) % stride;

			indices.push_back(i * stride + j);
			indices.push_back(nextI * stride + j);
			indices.push_back(i * stride + nextJ);

			indices.push_back(i * stride + nextJ);
			indices.push_back(nextI * stride + j);
			indices.push_back(nextI * stride + nextJ);
		}
	}

	InvertIndices(indices);

	FillRender(render, 
		vertices.data(), vertices.size(), 
		indices.data(), indices.size());
}

void GeometryRenderBuilder::FillRender(RenderComp& render,
                                       const RenderComp::Vertex* vertices, int verticesCount,
                                       const int* indices, int indicesCount)
{
	render.VerticesCount = verticesCount;
	render.Vertices = std::make_unique<RenderComp::Vertex[]>(verticesCount);
	for (int i = 0; i < verticesCount; i++)
		render.Vertices[i] = vertices[i];

	render.IndicesCount = indicesCount;
	render.Indices = std::make_unique<int[]>(indicesCount);
	for (int i = 0; i < indicesCount; i++)
		render.Indices[i] = indices[i];
}

void GeometryRenderBuilder::InvertIndices(std::vector<int>& indices)
{
	assert(indices.size() % 3 == 0);
	int pairsCount = indices.size() / 3;
	for (int i = 0; i < indices.size(); i+=3)
	{
		int temp = indices[i];
		indices[i] = indices[i + 2];
		indices[i + 2] = temp;
	}
}
