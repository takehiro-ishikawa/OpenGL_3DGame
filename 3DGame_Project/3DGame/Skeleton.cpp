#include "SkeletalAnimation.h"
#include "FBXData.h"

bool Skeleton::Load(FBXData* fbxFile)
{	
	// スケルトンのボーン配列を取得
	if (!fbxFile->ComputeBindPoses(mBones)) return false;

	// 全てのボーンのグローバルな逆バインドポーズ行列を計算する
	ComputeGlobalInvBindPose();

	return true;
}

void Skeleton::ComputeGlobalInvBindPose()
{
	// ボーンの数で配列の要素数を変更する。これにより、自動的にIDが記入される
	mGlobalInvBindPoses.resize(GetNumBones());

	// 各行列の逆行列を求める
	for (size_t i = 0; i < mGlobalInvBindPoses.size(); i++)
	{
		mGlobalInvBindPoses[i] = mBones[i].mMatrix;
		mGlobalInvBindPoses[i].Invert();
	}
}