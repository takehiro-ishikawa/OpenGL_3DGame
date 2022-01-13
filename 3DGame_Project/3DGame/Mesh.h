#pragma once
#include <vector>
#include <string>
#include "PhysWorld.h"
#include "Component.h"

#pragma region プロトタイプ宣言
class Actor;
class Shader;
class Mesh;
class Renderer;
class Texture;
class FBXData;
#pragma endregion

// 頂点バッファ、インデックスバッファ、頂点レイアウトをまとめて管理する
class VertexArray
{
public:
	// サポートされている頂点レイアウト
	enum class Layout
	{
		PosNormTex,    // 頂点位置、頂点法線、テクスチャ座標
		PosNormSkinTex // 頂点位置、頂点法線、スキニングのボーン、スキニングの重み、テクスチャ座標
	};

	VertexArray(const void* verts, unsigned int numVerts, Layout layout,
		const unsigned int* indices, unsigned int numIndices);
	VertexArray(const void* verts, unsigned int numVerts, const unsigned int* indices, unsigned int numIndices,
		const void* worldMatrices);
	~VertexArray();

	void SetActive();
	unsigned int GetNumIndices() const { return mNumIndices; }
	unsigned int GetNumVerts()   const { return mNumVerts; }

private:
	// 頂点バッファにある頂点の数
	unsigned int mNumVerts;

	// インデックスバッファ内のインデックスの数
	unsigned int mNumIndices;

	// OpenGL ID
	unsigned int mVertexBuffer;  // 頂点バッファ
	unsigned int mIndexBuffer;   // インデックスバッファ
	unsigned int mVertexArray;   // 頂点配列オブジェクト

	// インスタンス毎のワールド変換行列用バッファ(インスタンシング描画を行う時のみ使用)
	unsigned int mWorldMatricesID; 
};

namespace
{
	union Vertex
	{
		float f;
		uint8_t b[4];
	};
}

// 1つのメッシュデータを管理
class Mesh
{
public:
	Mesh();
	~Mesh();

	// メッシュのロード/アンロード
	virtual bool Load(FBXData* fbxFile, Renderer* renderer);
	virtual void Unload();

	// ゲッター
	VertexArray* GetVertexArray() { return mVertexArray; }           // このメッシュに関連付けられている頂点配列を取得
	Texture* GetTexture(size_t index);                               // インデックスからテクスチャを取得
	const std::string& GetShaderName() const { return mShaderName; } // シェーダーの名前を取得
	float GetRadius() const { return mRadius; }                      // オブジェクト空間での境界球の半径を取得
	const AABB& GetBox() const { return mBox; }                      // オブジェクト空間のAABBを取得
	float GetSpecPower() const { return mSpecPower; }                // メッシュの鏡面反射値を取得

protected:
	// AABB collision
	AABB mBox;

	// メッシュのテクスチャ群
	std::vector<Texture*> mTextures;

	// メッシュの頂点配列
	class VertexArray* mVertexArray;

	// シェーダーの名前
	std::string mShaderName;

	// オブジェクト空間での境界球の半径を記録
	float mRadius;

	// 表面の鏡面反射値
	float mSpecPower;
};


// メッシュのコンポーネント
class MeshComponent : public Component
{
public:
	MeshComponent(Actor* owner, bool isSkeletal);
	~MeshComponent();

	// このメッシュコンポーネントを描画する
	virtual void Draw(Shader* shader);

	// メッシュコンポーネントで使用されるメッシュ/テクスチャインデックスを設定する
	virtual void SetMesh(Mesh* mesh) { mMesh = mesh; }
	virtual Mesh* GetMesh() { return mMesh; }
	void SetTextureIndex(size_t index) { mTextureIndex = index; }

	void SetVisible(bool visible) { mVisible = visible; }
	bool GetVisible() const { return mVisible; }

	bool GetIsSkeletal() const { return mIsSkeletal; }

protected:
	Mesh* mMesh;
	size_t mTextureIndex;
	bool mVisible;
	bool mIsSkeletal;
};

// インスタンシング描画用のメッシュコンポーネント
class InstancedMeshComponent : public Component
{
public:
	InstancedMeshComponent(Actor* owner);
	~InstancedMeshComponent();

private:

};

struct WorldMatrix
{
	float mat0;
	float mat1;
	float mat2;
	float mat3;
	float mat4;
	float mat5;
	float mat6;
	float mat7;
	float mat8;
	float mat9;
	float mat10;
	float mat11;
	float mat12;
	float mat13;
	float mat14;
	float mat15;
};

class InstancedMesh : Mesh
{
public:
	InstancedMesh();
	~InstancedMesh();

	bool Load(FBXData* fbxFile, Renderer* renderer)override;
	void Unload()override;

	void AddInstance(const Matrix4& worldMat);

	void Draw();

private:

	std::vector<WorldMatrix> mWorldMatrices;
};