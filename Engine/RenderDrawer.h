#pragma once
#include "RenderComp.h"
#include "TransformComp.h"

namespace BlahEngine
{
class RenderDrawer
{
public:
	RenderDrawer();
	~RenderDrawer();
	
	void Draw(ID3D11Device* device, ID3D11DeviceContext* context, const TransformComp& tf, RenderComp& render);
private:
};
}
