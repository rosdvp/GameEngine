#pragma once

namespace BlahEngine
{
class RenderShader
{
public:
	virtual ~RenderShader() = default;

	virtual void Init(ID3D11Device* device);

	void ApplyToContext(ID3D11DeviceContext* context);

protected:
	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11InputLayout> _inputLayout;
	ComPtr<ID3D11PixelShader> _pixelShader;

	void CreateVertexShader(const WCHAR* fileName, LPCSTR entryPoint, LPCSTR shaderModel,
	                        ID3D11Device* device, D3D11_INPUT_ELEMENT_DESC* layoutDesc, int layoutDescNum);

	void CreatePixelShader(const WCHAR* fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3D11Device* device);

private:
	void CompileShader(const WCHAR* fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** outBlob);
};
}