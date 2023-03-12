#include "AnimatedRenderBackground.h"

void AnimatedRenderBackground::DrawFrameImpl(ID3D11DeviceContext* context, ID3D11RenderTargetView* renderTargetView)
{
	static float red = 0.0f;

	red += _timeModule->GetFrameDeltaSecs();

	if (red >= 1.0f)
		red = 0.0f;

	float color[] = { red, 0.1f, 0.1f, 1.0f };
	context->ClearRenderTargetView(renderTargetView, color);
}
