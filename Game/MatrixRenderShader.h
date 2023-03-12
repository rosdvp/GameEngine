#pragma once

#include "BlahEngine.h"

class MatrixRenderShader : public BlahEngine::RenderShader
{
public:
	void Init(ID3D11Device* device) override;
};