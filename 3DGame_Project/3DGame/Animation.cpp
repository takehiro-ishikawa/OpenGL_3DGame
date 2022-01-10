#include "SkeletalAnimation.h"
#include "FBXData.h"

bool Animation::Load(FBXData* fbxFile, const std::string& animationName)
{
	bool result = fbxFile->ComputeAnimation(animationName, mAnimInfo, mTracks);

	return result;
}

void Animation::GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const Skeleton* inSkeleton, float inTime) const
{
	// 必要ならば、outPose配列の要素数を再設定する
	if (outPoses.size() != mAnimInfo.NumBones)
	{
		outPoses.resize(mAnimInfo.NumBones);
	}

	// 現在と次のフレームインデックスを求める
	// （inTimeの範囲を[0、AnimDuration]と想定)
	size_t frame = static_cast<size_t>(inTime / mAnimInfo.FrameDuration);
	size_t nextFrame = (frame == mTracks[0].size() - 1)? 0 : frame + 1;
	
	// frameとnextFrameの間の小数値を計算
	float pct = inTime / mAnimInfo.FrameDuration - frame;

	const std::vector<BoneTransform>& bones = inSkeleton->GetBones();

	// 他の全てのボーンのグローバルポーズ行列を計算する
	for (size_t bone = 0; bone < mAnimInfo.NumBones; bone++)
	{
		const auto& boneTransforms = mTracks[bone];
		outPoses[bone] = boneTransforms[frame % boneTransforms.size()].mMatrix;
	}
}
