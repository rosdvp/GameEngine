#pragma once
#include "RenderComp.h"
#include "TransformComp.h"

namespace BlahEngine
{
class RenderObjectDrawer
{
public:
	RenderObjectDrawer() = default;
	~RenderObjectDrawer();

	void Init(ID3D11Device* device, ID3D11DeviceContext* context);

	void Draw(const TransformComp& tf, RenderComp& render);
private:
	ID3D11Device* _device;
	ID3D11DeviceContext* _context;

	void CreateVertexBuffer(RenderComp& render);
	void CreateIndexBuffer(RenderComp& render);

	void CreateTransformConstantBuffer(RenderComp& render);
	void UpdateTransformConstantBuffer(const TransformComp& tf, const RenderComp& render);

	void CreateMaterialConstantBuffer(RenderComp& render);
	void UpdateMaterialConstantBuffer(const RenderComp& render);

	struct TransformConstantBufferData
	{
		DirectX::XMMATRIX WorldMatrix;
	};
};
}
