#include "pch.h"
#include "RenderObjectDrawer.h"

#include "RenderSlotsConsts.h"
#include "Logger.h"
#include "RenderComp.h"
#include "TransformComp.h"

using namespace BlahEngine;
using namespace DirectX;


RenderObjectDrawer::~RenderObjectDrawer()
{
	Logger::Debug("render module", "object drawer destroyed");
}

void RenderObjectDrawer::Init(ID3D11Device* device, ID3D11DeviceContext* context)
{
	_device = device;
	_context = context;

	Logger::Debug("render module", "object drawer is initialized");
}

void RenderObjectDrawer::BeginFrame(const TransformComp& tf, RenderComp& render)
{
	bool shouldUpdateTransform = false;
	bool shouldUpdateMaterial = false;

	if (render.VertexBuffer.Get() == nullptr)
	{
		CreateVertexBuffer(render);
		CreateIndexBuffer(render);
		CreateTransformConstantBuffer(render);
		CreateMaterialConstantBuffer(render);

		shouldUpdateTransform = true;
		shouldUpdateMaterial = true;
	}
	if (shouldUpdateTransform || tf.IsChanged())
	{
		UpdateTransformConstantBuffer(tf, render);
	}
	if (shouldUpdateMaterial)
	{
		UpdateMaterialConstantBuffer(render);
	}
}

void RenderObjectDrawer::DrawFrame(RenderComp& render)
{
	_context->VSSetConstantBuffers(
		SLOT_TRANSFORM_CONST_BUFFER,
		1,
		render.TransformConstantBuffer.GetAddressOf()
	);
	_context->PSSetConstantBuffers(
		SLOT_MATERIAL_CONST_BUFFER,
		1, 
		render.MaterialConstantBuffer.GetAddressOf()
	);
	_context->PSSetShaderResources(SLOT_MAIN_TEX, 1, render.Texture.GetAddressOf());
		
	UINT stride = sizeof(RenderComp::Vertex);
	UINT offset = 0;
	
	_context->IASetVertexBuffers(
		0,
		1,
		 render.VertexBuffer.GetAddressOf(),
		&stride,
		&offset);

	_context->IASetIndexBuffer(render.IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	_context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	_context->DrawIndexed(render.IndicesCount, 0, 0);
}

void RenderObjectDrawer::CreateVertexBuffer(RenderComp& render)
{
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(RenderComp::Vertex) * render.VerticesCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexInitData = {};
	vertexInitData.pSysMem = render.Vertices.get();

	HRESULT result = _device->CreateBuffer(&vertexBufferDesc, &vertexInitData, render.VertexBuffer.GetAddressOf());
	if (FAILED(result))
		throw std::exception("failed to create vertex buffer");
}

void RenderObjectDrawer::CreateIndexBuffer(RenderComp& render)
{
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(int) * render.IndicesCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA indexInitData = {};
	indexInitData.pSysMem = render.Indices.get();

	HRESULT result = _device->CreateBuffer(&indexBufferDesc, &indexInitData, render.IndexBuffer.GetAddressOf());
	if (FAILED(result))
		throw std::exception("failed to create index buffer");
}

void RenderObjectDrawer::CreateTransformConstantBuffer(RenderComp& render)
{
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = sizeof(TransformConstantBufferData);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;

	HRESULT result = _device->CreateBuffer(
		&constantBufferDesc,
		nullptr,
		render.TransformConstantBuffer.GetAddressOf());
	if (FAILED(result))
		throw std::exception("failed to create matrix constant buffer");
}

void RenderObjectDrawer::UpdateTransformConstantBuffer(const TransformComp& tf, const RenderComp& render)
{
	auto worldMatrix =
		XMMatrixScaling(tf.Scale.X, tf.Scale.Y, tf.Scale.Z) *
		//XMMatrixRotationRollPitchYaw(tf.Rot.X, tf.Rot.Y, tf.Rot.Z) *
		//XMMatrixRotationRollPitchYaw(rot.X, rot.Y, rot.Z) *
		XMMatrixRotationQuaternion(tf.GlobalRot.GetQuaternion()) *
		XMMatrixTranslation(tf.GlobalPos.X, tf.GlobalPos.Y, tf.GlobalPos.Z);

	TransformConstantBufferData data =
	{
		XMMatrixTranspose(worldMatrix)
	};

	_context->UpdateSubresource(
		render.TransformConstantBuffer.Get(),
		0,
		nullptr,
		&data,
		0,
		0);
}

void RenderObjectDrawer::CreateMaterialConstantBuffer(RenderComp& render)
{
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = sizeof(RenderComp::Material);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;

	HRESULT result = _device->CreateBuffer(
		&constantBufferDesc,
		nullptr,
		render.MaterialConstantBuffer.GetAddressOf());
	if (FAILED(result))
		throw std::exception("failed to create material constant buffer");
}

void RenderObjectDrawer::UpdateMaterialConstantBuffer(const RenderComp& render)
{
	_context->UpdateSubresource(
		render.MaterialConstantBuffer.Get(),
		0,
		nullptr,
		&render.Mat,
		0,
		0);
}
