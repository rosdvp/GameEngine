#include "pch.h"
#include "RenderBackground.h"

using namespace BlahEngine;

RenderBackground::RenderBackground() :
	_timeModule(nullptr)
{}

void RenderBackground::Init(ID3D11Device* device, const TimeModule* timeModule)
{
	_timeModule = timeModule;

	InitImpl(device);
}

void RenderBackground::DrawFrame(ID3D11DeviceContext* context, ID3D11RenderTargetView* renderTargetView)
{
	DrawFrameImpl(context, renderTargetView);
}

void RenderBackground::InitImpl(ID3D11Device* device) { }

void RenderBackground::DrawFrameImpl(ID3D11DeviceContext* context, ID3D11RenderTargetView* renderTargetView)
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	context->ClearRenderTargetView(renderTargetView, clearColor);
}
