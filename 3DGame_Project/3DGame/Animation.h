#pragma once
#include "BoneTransform.h"
#include <vector>
#include <string>

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
	bool Load(const std::string& fileName, const std::string& animationName);

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