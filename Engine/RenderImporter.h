#pragma once

namespace BlahEngine
{
struct RenderComp;

class RenderImporter
{
public:
	void ImportModel(std::string fileName, float scaleFactor, RenderComp& render);

	void ImportTexture(ID3D11Device* device, std::wstring fileName, RenderComp& render);

private:
};
}
