#pragma once
#include "TimeModule.h"


namespace BlahEngine
{
class RenderBackground
{
public:
	RenderBackground();

	virtual ~RenderBackground() = default;

	void Init(ID3D11Device* device, const TimeModule* timeModule);

	void DrawFrame(ID3D11DeviceContext* context, ID3D11RenderTargetView* renderTargetView);

protected:
	const TimeModule* _timeModule;

	virtual void InitImpl(ID3D11Device* device);

	virtual void DrawFrameImpl(ID3D11DeviceContext* context, ID3D11RenderTargetView* renderTargetView);
};
}