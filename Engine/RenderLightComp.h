#pragma once
#include "Color.h"

namespace BlahEngine
{
struct RenderLightComp
{
	Color Color {0, 0, 0, 0};

	bool IsDirty = true;
};
}
