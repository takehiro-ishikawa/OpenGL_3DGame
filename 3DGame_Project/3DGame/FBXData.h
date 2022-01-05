#pragma once
#include <fbxsdk.h>
#include <iostream>
#include "SkeletalAnimation.h"

// プロトタイプ宣言
class BoneTransform;

// 頂点に影響を与えるボーンの情報
typedef struct
{
	std::vector<uint8_t> BoneIndex;  // ボーンのインデックス番号(ボーン数は1～255以内)
	std::vector<uint8_t> BoneWeight; // ボーンの重み(1～255以内)
}VertexBoneInfo;

// FBXSDKを用いてFBXファイルのデータを管理する
class FBXData
{
public:
	FBXData(const char* filename);
	~FBXData();

	void ComputeVertexData();                                     // メッシュの頂点情報を計算する
	bool ComputeBindPoses(std::vector<BoneTransform>& bindPoses); // スケルトンのボーン配列を計算して返す

	// アニメーションを取得する
	bool ComputeAnimation(const std::string& animationName, AnimInfo& animInfo,
		std::vector<std::vector<BoneTransform>>& tracks);

	// ゲッター
	int GetNumIndexCount() { return mNumIndexCount; }
	int GetNumVertexCount() { return mNumVertexCount; }
	std::vector<unsigned int> GetIndices() { return mIndices; }
	std::vector<Vector3>& GetVertices() { return mVertices; }
	std::vector<Vector3>& GetNormals() { return mNormals; }
	std::vector<Vector2>& GetUVs() { return mUVs; }
	std::vector<VertexBoneInfo>& GetVertexBoneInfos() { return mVertexBoneInfos; }
	bool GetIsSkeletal() { return mIsSkeletal; }
	std::string GetTextureName() { return mTextureName; }

private:

	// FBXのメッシュ情報を取得
	FbxMesh* GetFBXMesh(FbxNode* root_node);

	// メッシュのテクスチャを取得する
	void ComputeTextureName();

	// 頂点インデックスの取得
	void ComputeIndexList();

	// 頂点位置座標の取得
	void ComputePositionList();

	// 法線情報を取得
	void ComputeNormalList();

	// UV情報を取得
	void ComputeUVList(int uvNo);

	// テクスチャの名前を相対パスに修正する
	void FixTextureName(const char* textureName);

	std::string FixAnimationName(const char* animationName);

	// ボーンが影響を与える頂点の情報を取得する
	void ComputeBoneVertexInfo();

	// ラッパー関数
	// FBXAMatrixをMatrix4に変換する
	Matrix4& ConvertFBXMatrix(const FbxAMatrix& fbxMat);
	// Matrix4をBoneTransformに変換する
	BoneTransform& ConvertMatToBT(const Matrix4& mat);

	// FBXSDK関連
	FbxManager* mManager;
	FbxImporter* mImporter;
	FbxScene* mScene;

	FbxNode* mRoot_node; // ルートノード
	FbxMesh* mFbxMesh;   // メッシュ情報

	// メッシュ情報
	std::vector<unsigned int> mIndices;           // 頂点インデックス
	int	mNumIndexCount;                           // 頂点インデックス内のインデックスの数
    std::vector<Vector3> mVertices;               // 頂点位置座標データ
	int mNumVertexCount;                          // 頂点数
	std::vector<Vector3> mNormals;                // 法線情報
	std::vector<Vector2> mUVs;                    // UV情報
	std::string mTextureName;                     // テクスチャ名

	// スケルトン情報
	bool mIsSkeletal;                             // 骨格情報を持っているか？
	std::vector<VertexBoneInfo> mVertexBoneInfos; // 頂点に影響を与えるボーンの情報
};