#pragma once

namespace BlahEngine
{
class RenderLightDrawer
{
public:
	RenderLightDrawer() = default;
	~RenderLightDrawer();

	void Init(entt::registry* ecs, ID3D11Device* device, ID3D11DeviceContext* context);

	void Draw();
private:
	entt::registry* _ecs;

	ID3D11Device* _device;
	ID3D11DeviceContext* _context;

	ComPtr<ID3D11Buffer> _lightConstantBuffer;

	struct LightConstantBufferData
	{
		float AmbientIntensity;
		DirectX::XMFLOAT3 LightDir;
		DirectX::XMFLOAT4 LightColor;
	};
};
}
