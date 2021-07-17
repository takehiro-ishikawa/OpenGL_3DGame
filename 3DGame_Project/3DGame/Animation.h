#pragma once
#include "BoneTransform.h"
#include <vector>
#include <string>

// �A�j���[�V�����̏��
typedef struct
{
	size_t NumBones;     // �A�j���[�V�����̃{�[����
	size_t NumFrames;    // �A�j���[�V�����̃t���[����
	float Duration;      // �A�j���[�V�����̒����i�b�P�ʁj
	float FrameDuration; // �A�j���[�V�����̊e�t���[���̒���
}AnimInfo;

// 1�̃A�j���[�V�����f�[�^���Ǘ�
class Animation
{
public:
	bool Load(const std::string& fileName, const std::string& animationName);

	size_t GetNumBones() const { return mAnimInfo.NumBones; }
	size_t GetNumFrames() const { return mAnimInfo.NumFrames; }
	float GetDuration() const { return mAnimInfo.Duration; }
	float GetFrameDuration() const { return mAnimInfo.FrameDuration; }

	// �n���ꂽ�z��ɃO���[�o���|�[�Y�s����L������B
	// �w�肳�ꂽ�����ɂ�����A�e�{�[���̌��݂̃O���[�o���|�[�Y�̍s��ł���
	void GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const class Skeleton* inSkeleton, float inTime) const;

private:
	// �A�j���[�V�����̏��
	AnimInfo mAnimInfo;

	// �e�t���[���̕ϊ������g���b�N�Ɋi�[
	// �O���̔z��̃C���f�b�N�X�̓{�[��
	// �����̔z��̃C���f�b�N�X�̓t���[��
	std::vector<std::vector<BoneTransform>> mTracks;
};