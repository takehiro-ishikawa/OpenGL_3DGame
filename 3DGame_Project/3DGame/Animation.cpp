#include "Animation.h"
#include "Skeleton.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL/SDL_log.h>
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

	// ルートのポーズを設定する
	// ルートはトラックを持っているか？
	//if (mTracks[0].size() > 0)
	//{
	//	// 現在のフレームのポーズと次のフレームの間を補間する
	//	BoneTransform interp = BoneTransform::Interpolate(mTracks[0][frame],
	//		mTracks[0][nextFrame], pct);
	//	outPoses[0] = interp.ToMatrix();
	//}
	//else
	//{
	//	outPoses[0] = Matrix4::Identity;
	//}

	// 　　　　　　　　　　↓元はBone構造体だった
	const std::vector<BoneTransform>& bones = inSkeleton->GetBones();

	// 他の全てのボーンのグローバルポーズ行列を計算する
	for (size_t bone = 0; bone < mAnimInfo.NumBones; bone++)
	{
		//Matrix4 localMat; // (デフォルトでは単位行列)
		//if (mTracks[bone].size() > 0)
		//{
		//	BoneTransform interp = BoneTransform::Interpolate(mTracks[bone][frame],
		//		mTracks[bone][nextFrame], pct);
		//	localMat = interp.ToMatrix();
		//}

		outPoses[bone] = mTracks[bone][frame].mMatrix;
	}
}
