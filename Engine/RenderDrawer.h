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
	
	void DrawEntity(ID3D11Device* device, ID3D11DeviceContext* context, const TransformComp& transComp, RenderComp& renderComp);
private:
};
}
