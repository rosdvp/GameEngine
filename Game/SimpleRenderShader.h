#pragma once

#include "BlahEngine.h"

class SimpleRenderShader : public BlahEngine::RenderShader
{
public:
	void Init(ID3D11Device* device) override;
};