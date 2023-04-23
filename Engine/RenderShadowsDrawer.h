#pragma once
#include "RenderShaderShadowMap.h"

namespace BlahEngine
{
struct TransformComp;
struct RenderLightComp;

class RenderShadowsDrawer
	{
	public:

		~RenderShadowsDrawer();

		void Init(entt::registry* ecs, ID3D11Device* device, ID3D11DeviceContext* context);

		void BeginFrame();
		void DrawFrame(ID3D11RenderTargetView* renderTarget);

		void ApplyShadowMapToContext();

	private:
		static constexpr UINT SHADOWMAP_WIDTH = 4096;
		static constexpr UINT SHADOWMAP_HEIGHT = 4096;

		entt::registry* _ecs = nullptr;

		ID3D11Device* _device = nullptr;
		ID3D11DeviceContext* _context = nullptr;


		ComPtr<ID3D11Texture2D> _shadowMap;
		ComPtr<ID3D11DepthStencilView> _shadowDepthView;
		ComPtr<ID3D11ShaderResourceView> _shadowResView;
		ComPtr<ID3D11RenderTargetView> _shadowRenderTarget;
		ComPtr<ID3D11RasterizerState> _shadowRasterizer;
		D3D11_VIEWPORT _shadowViewport{};

		ComPtr<ID3D11Buffer> _shadowConstantBuffer;

		RenderShaderShadowMap _shader;

		void CreateShadowMap();

		void CreateShadowConstantBuffer();
		void UpdateShadowConstantBuffer(DirectX::XMMATRIX viewProjMatrix);

		
		struct ShadowConstantBufferData
		{
			DirectX::XMMATRIX LightViewProjMatrix;
		};
	};
}
