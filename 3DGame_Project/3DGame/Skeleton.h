#pragma once
#include "BoneTransform.h"
#include <string>
#include <vector>

// 1�̃X�P���g���f�[�^���Ǘ�
class Skeleton
{
public:
	
	// �t�@�C������ǂݍ���
	bool Load(const std::string& fileName);

	// �Q�b�^�[
	size_t GetNumBones() const { return mBones.size(); }
	const BoneTransform& GetBone(size_t idx) const { return mBones[idx]; }
	const std::vector<BoneTransform>& GetBones() const { return mBones; }
	const std::vector<Matrix4>& GetGlobalInvBindPoses() const { return mGlobalInvBindPoses; }

protected:
	// ���ׂẴ{�[���́A�O���[�o���ȋt�o�C���h�|�[�Y�s����v�Z����
	// �i�X�P���g���̃��[�h���ɌĂяo�����j
	void ComputeGlobalInvBindPose();

private:
	// �X�P���g���̃{�[���z��
	std::vector<BoneTransform> mBones;

	// �{�[���̃O���[�o���ȋt�o�C���h�|�[�Y�s��
	std::vector<Matrix4> mGlobalInvBindPoses;
};