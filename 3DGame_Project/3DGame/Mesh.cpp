#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL/SDL_log.h>
#include "Math.h"
#include <fbxsdk.h>
#include "FBXAnalyze.h"
#include <iostream>

namespace
{
	union Vertex
	{
		float f;
		uint8_t b[4];
	};
}

Mesh::Mesh()
	:mBox(Vector3::Infinity, Vector3::NegInfinity)
	, mVertexArray(nullptr)
	, mRadius(0.0f)
	, mSpecPower(100.0f)
{
}

Mesh::~Mesh()
{
}

bool Mesh::Load(const std::string& fileName, Renderer* renderer)
{
	FBXAnalyze* fbxFile = new FBXAnalyze(fileName.c_str());

	// メッシュの頂点情報を計算
	fbxFile->ComputeVertexData();

	// テクスチャを取得
	std::string texName = fbxFile->GetTextureName();
	Texture* t = renderer->GetTexture(texName);

	// テクスチャが存在しないものだった場合
	if (t == nullptr)
	{
		// デフォルトのテクスチャを使用する
		t = renderer->GetTexture("Assets/Textures/Default.png");
	}
	mTextures.emplace_back(t);

	// 頂点配列に割り当てるデータの作成
	std::vector<Vertex> vertices;

	// 頂点から取得する情報を設定
	size_t vertSize;
	VertexArray::Layout layout;
	if (fbxFile->GetIsSkeletal())
	{
		// 位置、法線、ボーンのインデックス番号、ウェイト、UV座標
		vertSize = 10;
		layout = VertexArray::Layout::PosNormSkinTex;
		vertices.reserve(fbxFile->GetNumVertexCount() * (sizeof(float) * 8 + sizeof(uint8_t) * 8));
	}
	else
	{
		// 位置、法線、UV座標
		vertSize = 8;
		layout = VertexArray::Layout::PosNormTex;
		vertices.reserve(fbxFile->GetNumVertexCount() * sizeof(float) * 8);
	}
	
	std::vector<Vector3> positions = fbxFile->GetVertices();            // 位置座標
	std::vector<Vector3> normals = fbxFile->GetNormals();               // 法線
	std::vector<Vector2> uvs = fbxFile->GetUVs();                       // UV座標
	std::vector<VertexBoneInfo> VBInfo = fbxFile->GetVertexBoneInfos(); // ボーンの番号、ウェイト

	for (int i = 0; i < fbxFile->GetNumVertexCount(); i++)
	{
		Vertex v;

		// 位置座標
		v.f = static_cast<float>(positions[i].x);
		vertices.emplace_back(v);
		v.f = static_cast<float>(positions[i].y);
		vertices.emplace_back(v);
		v.f = static_cast<float>(positions[i].z);
		vertices.emplace_back(v);

		// 法線
		v.f = static_cast<float>(normals[i].x);
		vertices.emplace_back(v);
		v.f = static_cast<float>(normals[i].y);
		vertices.emplace_back(v);
		v.f = static_cast<float>(normals[i].z);
		vertices.emplace_back(v);

		// UV座標
		v.f = static_cast<float>(uvs[i].x);
		vertices.emplace_back(v);
		v.f = static_cast<float>(uvs[i].y);
		vertices.emplace_back(v);

		// ボーンを持つメッシュの場合
		if (fbxFile->GetIsSkeletal())
		{
			// 影響を与えるボーンのインデックス番号
			v.b[0] = static_cast<uint8_t>(VBInfo[i].BoneIndex[0]);
			v.b[1] = static_cast<uint8_t>(VBInfo[i].BoneIndex[1]);
			v.b[2] = static_cast<uint8_t>(VBInfo[i].BoneIndex[2]);
			v.b[3] = static_cast<uint8_t>(VBInfo[i].BoneIndex[3]);
			vertices.emplace_back(v);

			// ウェイト
			v.b[0] = static_cast<uint8_t>(VBInfo[i].BoneWeight[0]);
			v.b[1] = static_cast<uint8_t>(VBInfo[i].BoneWeight[1]);
			v.b[2] = static_cast<uint8_t>(VBInfo[i].BoneWeight[2]);
			v.b[3] = static_cast<uint8_t>(VBInfo[i].BoneWeight[3]);
			vertices.emplace_back(v);
		}

		// 境界球の半径とAABBの設定
		mRadius = Math::Max(mRadius, positions[i].LengthSq());
		mBox.UpdateMinMax(positions[i]);
	}
	// ----------------------------------------------------------------------------

	std::cout << "AABBのサイズ\n";
	/*std::cout << "最小 x = " << mBox.mMin.x << " y = " << mBox.mMin.y << " z = " << mBox.mMin.z << std::endl;
	std::cout << "最大 x = " << mBox.mMax.x << " y = " << mBox.mMax.y << " z = " << mBox.mMax.z << std::endl;*/
	std::cout << "x = " << mBox.mMax.x - mBox.mMin.x << " y = " << mBox.mMax.y - mBox.mMin.y << " z = " << mBox.mMax.z - mBox.mMin.z << std::endl;

	// 頂点配列を作成する
	mVertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size()) / vertSize,
		layout, fbxFile->GetIndices().data(), static_cast<unsigned>(fbxFile->GetNumIndexCount()));

	// 解放処理
	delete fbxFile;

	return true;
}

void Mesh::Unload()
{
	delete mVertexArray;
	mVertexArray = nullptr;
}

Texture* Mesh::GetTexture(size_t index)
{
	if (index < mTextures.size())
	{
		return mTextures[index];
	}
	else
	{
		return nullptr;
	}
}