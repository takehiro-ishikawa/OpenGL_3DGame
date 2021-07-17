#pragma once

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
