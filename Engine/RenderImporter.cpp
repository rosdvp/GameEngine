#include "pch.h"
#include "RenderImporter.h"

#include "Logger.h"
#include "RenderComp.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "directxtk/DDSTextureLoader.h"


using namespace BlahEngine;
using namespace DirectX;


void RenderImporter::ImportModel(std::string fileName, float scaleFactor, RenderComp& render)
{
	Assimp::Importer importer;
	
	const auto model = importer.ReadFile(fileName.data(),
		aiProcess_Triangulate
		| aiProcess_ConvertToLeftHanded
		| aiProcess_GenSmoothNormals
		| aiProcess_FindDegenerates
		| aiProcess_FindInvalidData
		| aiProcess_CalcTangentSpace
		| aiProcess_JoinIdenticalVertices
	);

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
		vertex.Pos =
		{
			mesh->mVertices[i].x * scaleFactor,
			mesh->mVertices[i].y * scaleFactor,
			mesh->mVertices[i].z * scaleFactor
		};
		vertex.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

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

	
	float maxLength = -1;
	for (int i = 0; i < render.VerticesCount; i++)
	{
		auto pos = render.Vertices[i].Pos;
		float length = std::sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);
		if (length > maxLength)
			maxLength = length;
	}

	for (int i = 0; i < render.VerticesCount; i++)
	{
		auto& pos = render.Vertices[i].Pos;
		pos.x /= maxLength;
		pos.y /= maxLength;
		pos.z /= maxLength;
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
