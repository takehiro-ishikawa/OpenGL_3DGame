#pragma once
#include "BoneTransform.h"
#include <string>
#include <vector>

// 1つのスケルトンデータを管理
class Skeleton
{
public:
	
	// ファイルから読み込む
	bool Load(const std::string& fileName);

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