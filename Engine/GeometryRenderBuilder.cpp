#include "pch.h"
#include "GeometryRenderBuilder.h"

using namespace BlahEngine;
using namespace DirectX;

void GeometryRenderBuilder::BuildSquare(RenderComp& render, const Color& color)
{
	render.VerticesCount = 4;
	render.Vertices = new RenderComp::Vertex[render.VerticesCount]
	{
		{XMFLOAT3(-1.0f, -1.0f, 0.5f), color},
		{XMFLOAT3(-1.0f, 1.0f, 0.5f), color},
		{XMFLOAT3(1.0f, 1.0f, 0.5f), color},
		{XMFLOAT3(1.0f, -1.0f, 0.5f), color},
	};
	render.IndicesCount = 6;
	render.Indices      = new int[render.IndicesCount]
	{
		0, 1, 2,
		0, 2, 3
	};
}


void GeometryRenderBuilder::BuildCube(RenderComp& render, const Color& color)
{
	render.VerticesCount = 8;
	render.Vertices      = new RenderComp::Vertex[render.VerticesCount]
	{
		{{-1.0f, 1.0f, -1.0f}, color},
		{{1.0f, 1.0f, -1.0f}, color},
		{{-1.0f, -1.0f, -1.0f}, color},
		{{1.0f, -1.0f, -1.0f}, color},
		{{-1.0f, 1.0f, 1.0f}, color},
		{{1.0f, 1.0f, 1.0f}, color},
		{{-1.0f, -1.0f, 1.0f}, color},
		{{1.0f, -1.0f, 1.0f}, color},
	};

	render.IndicesCount = 36;
	render.Indices      = new int[render.IndicesCount]
	{
		0, 1, 2, // side 1
		2, 1, 3,
		4, 0, 6, // side 2
		6, 0, 2,
		7, 5, 6, // side 3
		6, 5, 4,
		3, 1, 7, // side 4
		7, 1, 5,
		4, 5, 0, // side 5
		0, 5, 1,
		3, 7, 2, // side 6
		2, 7, 6,
	};
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

			vertices.push_back({ {dx, dy, dz}, color});
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

	render.VerticesCount = vertices.size();
	render.Vertices      = new RenderComp::Vertex[render.VerticesCount];
	for (int i             = 0; i < render.VerticesCount; i++)
		render.Vertices[i] = vertices[i];

	render.IndicesCount = indices.size();
	render.Indices      = new int[render.IndicesCount];
	for (int i            = 0; i < render.IndicesCount; i++)
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
