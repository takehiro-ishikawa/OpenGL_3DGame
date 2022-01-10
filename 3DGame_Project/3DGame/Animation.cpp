#include "SkeletalAnimation.h"
#include "FBXData.h"

bool Animation::Load(FBXData* fbxFile, const std::string& animationName)
{
	bool result = fbxFile->ComputeAnimation(animationName, mAnimInfo, mTracks);

	return result;
}

void Animation::GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const Skeleton* inSkeleton, float inTime) const
{
	// �K�v�Ȃ�΁AoutPose�z��̗v�f�����Đݒ肷��
	if (outPoses.size() != mAnimInfo.NumBones)
	{
		outPoses.resize(mAnimInfo.NumBones);
	}

	// ���݂Ǝ��̃t���[���C���f�b�N�X�����߂�
	// �iinTime�͈̔͂�[0�AAnimDuration]�Ƒz��)
	size_t frame = static_cast<size_t>(inTime / mAnimInfo.FrameDuration);
	size_t nextFrame = (frame == mTracks[0].size() - 1)? 0 : frame + 1;
	
	// frame��nextFrame�̊Ԃ̏����l���v�Z
	float pct = inTime / mAnimInfo.FrameDuration - frame;

	const std::vector<BoneTransform>& bones = inSkeleton->GetBones();

	// ���̑S�Ẵ{�[���̃O���[�o���|�[�Y�s����v�Z����
	for (size_t bone = 0; bone < mAnimInfo.NumBones; bone++)
	{
		const auto& boneTransforms = mTracks[bone];
		outPoses[bone] = boneTransforms[frame % boneTransforms.size()].mMatrix;
	}
}
