#pragma once

#include "BlahEngine.h"

class AnimatedRenderBackground : public BlahEngine::RenderBackground
{
protected:
	void DrawFrameImpl(
		ID3D11DeviceContext* context,
		ID3D11RenderTargetView* renderTargetView) override;
};