#pragma once
#include "RenderShader.h"

namespace BlahEngine
{
	class RenderShaderSimple : public RenderShader
	{
	public:
		void Init(ID3D11Device* device) override;
	};
}
