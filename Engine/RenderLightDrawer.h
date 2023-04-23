#pragma once

namespace BlahEngine
{
struct RenderLightComp;
struct TransformComp;

class RenderLightDrawer
{
public:
	RenderLightDrawer() = default;
	~RenderLightDrawer();

	void Init(entt::registry* ecs, ID3D11Device* device, ID3D11DeviceContext* context);

	void BeginFrame();

	void DrawFrame();
private:
	entt::registry* _ecs;

	ID3D11Device* _device;
	ID3D11DeviceContext* _context;

	ComPtr<ID3D11Buffer> _lightConstantBuffer;


	void CreateLightConstantBuffer();
	void UpdateLightConstantBuffer(const TransformComp& tf, const RenderLightComp& light);

	struct LightConstantBufferData
	{
		float AmbientIntensity;
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT3 Dir;
		DirectX::XMFLOAT4 Color;
		DirectX::XMMATRIX ViewProjMatrixForShadowMap;
	};
};
}
