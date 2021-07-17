#pragma once
#include <vector>
#include <string>
#include "Collision.h"

#pragma region プロトタイプ宣言
class Renderer;
class VertexArray;
class Texture;
#pragma endregion


class Mesh
{
public:
	Mesh();
	~Mesh();

	// メッシュのロード/アンロード
	virtual bool Load(const std::string& fileName, Renderer* renderer);
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