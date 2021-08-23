#pragma once
#include "Math.h"
#include "vector"
#include "string"
#include "Mesh.h"

// プロトタイプ宣言
class FBXData;
class Actor;

// ボーン変換を管理
class BoneTransform
{
public:
	// ボーンの持つ情報
	Vector3 mTranslation; // 位置
	Quaternion mRotation; // 方向
	Matrix4 mMatrix;

	// ボーンの情報を行列に変換する
	Matrix4 ToMatrix() const;

	// 二つのボーンを補間する
	static BoneTransform Interpolate(const BoneTransform& a, const BoneTransform& b, float f);
};


// 1つのスケルトンデータを管理
class Skeleton
{
public:
	// ファイルから読み込む
	bool Load(FBXData* fbxFile);

	// ゲッター
	size_t GetNumBones() const { return mBones.size(); }
	const BoneTransform& GetBone(size_t idx) const { return mBones[idx]; }
	const std::vector<BoneTransform>& GetBones() const { return mBones; }
	const std::vector<Matrix4>& GetGlobalInvBindPoses() const { return mGlobalInvBindPoses; }

protected:
	// すべてのボーンの、グローバルな逆バインドポーズ行列を計算する
	// （スケルトンのロード時に呼び出される）
	void ComputeGlobalInvBindPose();

private:
	// スケルトンのボーン配列
	std::vector<BoneTransform> mBones;

	// ボーンのグローバルな逆バインドポーズ行列
	std::vector<Matrix4> mGlobalInvBindPoses;
};


// アニメーションの情報
typedef struct
{
	size_t NumBones;     // アニメーションのボーン数
	size_t NumFrames;    // アニメーションのフレーム数
	float Duration;      // アニメーションの長さ（秒単位）
	float FrameDuration; // アニメーションの各フレームの長さ
}AnimInfo;

// 1つのアニメーションデータを管理
class Animation
{
public:
	bool Load(FBXData* fbxFile, const std::string& animationName);

	size_t GetNumBones() const { return mAnimInfo.NumBones; }
	size_t GetNumFrames() const { return mAnimInfo.NumFrames; }
	float GetDuration() const { return mAnimInfo.Duration; }
	float GetFrameDuration() const { return mAnimInfo.FrameDuration; }

	// 渡された配列にグローバルポーズ行列を記入する。
	// 指定された時刻における、各ボーンの現在のグローバルポーズの行列である
	void GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const class Skeleton* inSkeleton, float inTime) const;

private:
	// アニメーションの情報
	AnimInfo mAnimInfo;

	// 各フレームの変換情報をトラックに格納
	// 外側の配列のインデックスはボーン
	// 内側の配列のインデックスはフレーム
	std::vector<std::vector<BoneTransform>> mTracks;
};


// ボーンの最大値
const size_t MAX_SKELETON_BONES = 96;

// 行列パレット
struct MatrixPalette
{
	Matrix4 mEntry[MAX_SKELETON_BONES];
};


// スケルタルアニメーションを行うメッシュのコンポーネント
class SkeletalMeshComponent : public MeshComponent
{
public:
	SkeletalMeshComponent(Actor* owner);

	// メッシュコンポーネントを描画
	void Draw(Shader* shader) override;

	void Update(float deltaTime) override;

	// セッター
	void SetSkeleton(const Skeleton* sk) { mSkeleton = sk; }

	// アニメーションを再生し。アニメーションの長さを返す
	float PlayAnimation(const Animation* anim, float playRate = 1.0f);

protected:

	// 行列パレットを計算する
	void ComputeMatrixPalette();

	MatrixPalette mPalette;      // 行列パレット
	const Skeleton* mSkeleton;   // 自分が割り当てられるスケルトン
	const Animation* mAnimation; // 今再生しているアニメーション
	float mAnimPlayRate;         // アニメーションの再生レート（1.0が通常のスピード）
	float mAnimTime;             // アニメーションの現在の時刻
};