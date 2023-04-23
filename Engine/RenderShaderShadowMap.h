#pragma once
#include "RenderShader.h"

namespace BlahEngine
{
class RenderShaderShadowMap : public RenderShader
{
public:
	void Init(ID3D11Device* device) override;
};
}
