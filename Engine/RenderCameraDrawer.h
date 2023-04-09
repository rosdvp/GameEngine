#pragma once

namespace BlahEngine
{
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
};
}
