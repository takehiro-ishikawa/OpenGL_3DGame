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

	// ���[�g�̃|�[�Y��ݒ肷��
	// ���[�g�̓g���b�N�������Ă��邩�H
	//if (mTracks[0].size() > 0)
	//{
	//	// ���݂̃t���[���̃|�[�Y�Ǝ��̃t���[���̊Ԃ��Ԃ���
	//	BoneTransform interp = BoneTransform::Interpolate(mTracks[0][frame],
	//		mTracks[0][nextFrame], pct);
	//	outPoses[0] = interp.ToMatrix();
	//}
	//else
	//{
	//	outPoses[0] = Matrix4::Identity;
	//}

	// �@�@�@�@�@�@�@�@�@�@������Bone�\���̂�����
	const std::vector<BoneTransform>& bones = inSkeleton->GetBones();

	// ���̑S�Ẵ{�[���̃O���[�o���|�[�Y�s����v�Z����
	for (size_t bone = 0; bone < mAnimInfo.NumBones; bone++)
	{
		//Matrix4 localMat; // (�f�t�H���g�ł͒P�ʍs��)
		//if (mTracks[bone].size() > 0)
		//{
		//	BoneTransform interp = BoneTransform::Interpolate(mTracks[bone][frame],
		//		mTracks[bone][nextFrame], pct);
		//	localMat = interp.ToMatrix();
		//}

		outPoses[bone] = mTracks[bone][frame].mMatrix;
	}
}
