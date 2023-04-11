#pragma once

namespace BlahEngine
{
struct TransformComp;
struct RenderCameraComp;

class RenderCameraDrawer
{
public:
	RenderCameraDrawer() = default;
	~RenderCameraDrawer();

	void Init(entt::registry* ecs, ID3D11Device* device, ID3D11DeviceContext* context, int screenWidth,
	          int screenHeight);

	void Draw();

private:
	entt::registry* _ecs;

	int _screenWidth;
	int _screenHeight;

	ID3D11Device* _device;
	ID3D11DeviceContext* _context;

	void CreateCameraConstantBuffer(RenderCameraComp& camera);
	void UpdateCameraConstantBuffer(const TransformComp& tf, RenderCameraComp& camera);
	
	struct CameraConstantBufferData
	{
		DirectX::XMMATRIX CameraMatrix;
		DirectX::XMFLOAT3 CameraPos;
	};
};
}
