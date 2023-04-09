#pragma once
#include "RenderShader.h"

namespace BlahEngine
{
	class RenderShaderUnlit : public RenderShader
	{
	public:
		void Init(ID3D11Device* device) override;
	};
}
