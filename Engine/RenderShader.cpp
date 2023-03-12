#include "pch.h"
#include "RenderShader.h"

#include <d3dcompiler.h>

#include "Logger.h"

using namespace BlahEngine;

void RenderShader::ApplyToContext(ID3D11DeviceContext* context)
{
	context->IASetInputLayout(_inputLayout.Get());
	context->VSSetShader(_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(_pixelShader.Get(), nullptr, 0);
}


void RenderShader::CreateVertexShader(const WCHAR* fileName, LPCSTR entryPoint, LPCSTR shaderModel,
	ID3D11Device* device, D3D11_INPUT_ELEMENT_DESC* layoutDesc, int layoutDescNum)
{
	HRESULT result = S_OK;
	
	ID3DBlob* blob = nullptr;
	CompileShader(fileName, entryPoint, shaderModel, &blob);

	result = device->CreateVertexShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		_vertexShader.GetAddressOf());
	if (FAILED(result))
		throw std::exception(("failed to create vertex shader, " + 
			std::string(reinterpret_cast<const char*>(fileName))).data());

	Logger::Debug("shaders", "vertex shader created, %S", fileName);

	result = device->CreateInputLayout(
		layoutDesc,
		layoutDescNum,
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		_inputLayout.GetAddressOf());

	blob->Release();
	blob = nullptr;

	if (FAILED(result))
		throw std::exception(("failed to vertex shader input layout, " +
			std::string(reinterpret_cast<const char*>(fileName))).data());

	Logger::Debug("shaders", "vertex shader input layout created, %S", fileName);
}

void RenderShader::CreatePixelShader(const WCHAR* fileName, LPCSTR entryPoint, LPCSTR shaderModel,
	ID3D11Device* device)
{
	HRESULT result = S_OK;

	ID3DBlob* blob = nullptr;
	CompileShader(fileName, entryPoint, shaderModel, &blob);

	result = device->CreatePixelShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		_pixelShader.GetAddressOf());

	blob->Release();
	blob = nullptr;

	if (FAILED(result))
		throw std::exception(("failed to create pixel shader, " +
			std::string(reinterpret_cast<const char*>(fileName))).data());

	Logger::Debug("shaders", "pixel shader created, %S", fileName);
}


void RenderShader::CompileShader(const WCHAR* fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** outBlob)
{
	ID3DBlob* errorMsg = nullptr;

	HRESULT result = D3DCompileFromFile(fileName,
		nullptr /*macros*/,
		nullptr /*include*/,
		entryPoint,
		shaderModel,
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		outBlob,
		&errorMsg);

	if (FAILED(result))
	{
		if (errorMsg)
		{
			auto compileErrors = static_cast<char*>(errorMsg->GetBufferPointer());
			throw std::exception(("shader compile error, " +
				std::string(reinterpret_cast<const char*>(fileName)) +
				", " +
				compileErrors).data());
		}
		throw std::exception(("could not find shader, " +
			std::string(reinterpret_cast<const char*>(fileName))).data());
	}
}
