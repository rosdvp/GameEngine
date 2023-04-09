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
	context->PSSetSamplers(0, 1, _sampler.GetAddressOf());
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

void RenderShader::CreateSampler(ID3D11Device* device)
{
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	auto result = device->CreateSamplerState(&samplerDesc, _sampler.GetAddressOf());
	if (FAILED(result))
		throw std::exception("failed to create sampler");
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

			std::cout << "shader compile error (" << fileName << "), " << compileErrors << std::endl;
		}
		std::cout << "could not find shader (" << fileName << ")" << std::endl;
	}
}
