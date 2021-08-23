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
	~VertexArray();

	void SetActive();
	unsigned int GetNumIndices() const { return mNumIndices; }
	unsigned int GetNumVerts()   const { return mNumVerts; }

private:
	// 頂点バッファにある頂点の数
	unsigned int mNumVerts;

	// インデックスバッファ内のインデックスの数
	unsigned int mNumIndices;

	// 頂点バッファのOpenGL ID
	unsigned int mVertexBuffer;

	// インデックスバッファのOpenGL ID
	unsigned int mIndexBuffer;

	// 頂点配列オブジェクトのOpenGL ID
	unsigned int mVertexArray;
};


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

private:

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