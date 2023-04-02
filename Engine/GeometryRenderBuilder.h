#pragma once
#include "Color.h"
#include "RenderComp.h"

namespace BlahEngine
{
class GeometryRenderBuilder
{
public:
	static void BuildSquare(RenderComp& render, const Color& color);
	static void BuildCube(RenderComp& render, const Color& color);
	static void BuildSphere(RenderComp& render, size_t tessellation, const Color& color);

private:
	static void FillRender(RenderComp& render,
	                       const RenderComp::Vertex* vertices, int verticesCount,
	                       const int* indices, int indicesCount);

	static void InvertIndices(std::vector<int>& indices);
};
}
