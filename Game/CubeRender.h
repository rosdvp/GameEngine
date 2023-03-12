#pragma once
#include "Engine.h"

#include <directxmath.h>

using namespace DirectX;

class CubeRender : public OldRender
{
public:
	CubeRender();

	bool Draw() override;
	
	void* operator new(size_t i)
	{
		return _aligned_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_aligned_free(p);
	}


protected:
	bool Init(HWND hwnd) override;
	void Close() override;

private:
	ID3D11Buffer* _vertexBuffer;
	ID3D11InputLayout* _vertexLayout;
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;

	XMMATRIX _matrixWorld;
	XMMATRIX _matrixView;
	XMMATRIX _matrixProjection;

	ID3D11Buffer* _indexBuffer;
	ID3D11Buffer* _constantBuffer;
};