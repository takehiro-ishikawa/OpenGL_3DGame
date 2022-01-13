#include "Mesh.h"
#include "Renderer.h"
#include "FBXData.h"
#include "Texture.h"
#include <GL/glew.h>

InstancedMesh::InstancedMesh()
	:Mesh()
{
	mWorldMatrices.reserve(10000);
}

InstancedMesh::~InstancedMesh(){}

bool InstancedMesh::Load(FBXData* fbxFile, Renderer* renderer)
{
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
	// 位置、法線、UV座標
	vertSize = 8;
	vertices.reserve(fbxFile->GetNumVertexCount() * sizeof(float) * 8);

	std::vector<Vector3> positions = fbxFile->GetVertices();            // 位置座標
	std::vector<Vector3> normals = fbxFile->GetNormals();               // 法線
	std::vector<Vector2> uvs = fbxFile->GetUVs();                       // UV座標

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

		// 境界球の半径とAABBの設定
		mRadius = Math::Max(mRadius, positions[i].LengthSq());
		mBox.UpdateMinMax(positions[i]);
	}
	// ----------------------------------------------------------------------------

	// 頂点配列を作成する
	mVertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size()) / vertSize,
		fbxFile->GetIndices().data(), static_cast<unsigned>(fbxFile->GetNumIndexCount()), mWorldMatrices.data());

	return true;
}

void InstancedMesh::Unload()
{
	delete mVertexArray;
	mVertexArray = nullptr;

	mWorldMatrices.clear();
}

void InstancedMesh::AddInstance(const Matrix4& worldMat)
{
	WorldMatrix mat;
	mat.mat0  = worldMat.mat[0][0];
	mat.mat1  = worldMat.mat[0][1];
	mat.mat2  = worldMat.mat[0][2];
	mat.mat3  = worldMat.mat[0][3];
	mat.mat4  = worldMat.mat[1][0];
	mat.mat5  = worldMat.mat[1][1];
	mat.mat6  = worldMat.mat[1][2];
	mat.mat7  = worldMat.mat[1][3];
	mat.mat8  = worldMat.mat[2][0];
	mat.mat9  = worldMat.mat[2][1];
	mat.mat10 = worldMat.mat[2][2];
	mat.mat11 = worldMat.mat[2][3];
	mat.mat12 = worldMat.mat[3][0];
	mat.mat13 = worldMat.mat[3][1];
	mat.mat14 = worldMat.mat[3][2];
	mat.mat15 = worldMat.mat[3][3];

	mWorldMatrices.emplace_back(mat);
}

void InstancedMesh::Draw()
{
	if (mWorldMatrices.size() <= 0) return;

	mTextures[0]->SetActive();

	mVertexArray->SetActive();

	glDrawElementsInstanced(GL_TRIANGLE_STRIP, mVertexArray->GetNumIndices(), 
		GL_UNSIGNED_INT, nullptr, mWorldMatrices.size());

	glBindVertexArray(0);
}