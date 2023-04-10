#pragma once
#include "RenderShader.h"

namespace BlahEngine
{
class RenderShaderLit : public RenderShader
{
public:
	void Init(ID3D11Device* device) override;
};
}
