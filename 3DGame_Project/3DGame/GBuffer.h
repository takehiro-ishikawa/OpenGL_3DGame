#pragma once
#include "vector"

// プロトタイプ宣言
class Texture;

class GBuffer
{
public:

	// Gバッファに格納されるデータの種類
	enum Type
	{
		EDiffuse = 0,
		ENormal,
		EWorldPos,
		NUM_GBUFFER_TEXTURES
	};

	GBuffer();
	~GBuffer();

	// Gバッファの生成と破棄
	bool Create(int width, int height);
	void Destroy();

	// 指定するタイプのテクスチャを取得
	Texture* GetTexture(Type type);

	// フレームバッファオブジェクトの ID を取得
	unsigned int GetBufferID() { return mBufferID; }

	// Gバッファの各テクスチャを対応するテクスチャインデックスにバインドする
	void SetTexturesActive();
	
private:

	// Gバッファに割り当てられたテクスチャ
	std::vector<Texture*> mTextures;

	// フレームバッファオブジェクトのID
	unsigned int mBufferID;
};