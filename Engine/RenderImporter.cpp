#include "pch.h"
#include "RenderImporter.h"

#include "Logger.h"
#include "RenderComp.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "directxtk/DDSTextureLoader.h"


using namespace BlahEngine;


void RenderImporter::ImportModel(std::string fileName, RenderComp& render)
{
	Assimp::Importer importer;

	const auto model = importer.ReadFile(fileName.data(),
		aiProcess_Triangulate | 
		aiProcess_ConvertToLeftHanded |
		aiProcess_CalcTangentSpace |
		aiProcess_JoinIdenticalVertices);

	if (model == nullptr)
	{
		Logger::Print("render module", ("failed to import (" + fileName + "), " + importer.GetErrorString()).data());
		return;
	}

	const auto mesh = model->mMeshes[0];
	render.VerticesCount = mesh->mNumVertices;
	render.Vertices = std::make_unique<RenderComp::Vertex[]>(render.VerticesCount);
	for (int i = 0; i < render.VerticesCount; i++)
	{
		RenderComp::Vertex vertex;
		vertex.Pos = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		vertex.Color = { 1.0f, 1.0f, 1.0f, 1.0f };

		if (mesh->mTextureCoords[0])
		{
			vertex.TexCoord.x = mesh->mTextureCoords[0][i].x;
			vertex.TexCoord.y = mesh->mTextureCoords[0][i].y;
		}
		else
			vertex.TexCoord = { 0.5f, 0.5f };

		render.Vertices[i] = vertex;
	}

	render.IndicesCount = mesh->mNumFaces * 3;
	render.Indices = std::make_unique<int[]>(render.IndicesCount);
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		const auto& face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);
		render.Indices[i * 3] = face.mIndices[0];
		render.Indices[i * 3 + 1] = face.mIndices[1];
		render.Indices[i * 3 + 2] = face.mIndices[2];
	}
}

void RenderImporter::ImportTexture(ID3D11Device* device, std::wstring fileName, RenderComp& render)
{
	DirectX::CreateDDSTextureFromFile(
		device,
		fileName.data(),
		nullptr,
		render.Texture.GetAddressOf());
}
