#include "SkeletalAnimation.h"
#include "FBXData.h"

bool Skeleton::Load(FBXData* fbxFile)
{	
	// �X�P���g���̃{�[���z����擾
	if (!fbxFile->ComputeBindPoses(mBones)) return false;

	// �S�Ẵ{�[���̃O���[�o���ȋt�o�C���h�|�[�Y�s����v�Z����
	ComputeGlobalInvBindPose();

	return true;
}

void Skeleton::ComputeGlobalInvBindPose()
{
	// �{�[���̐��Ŕz��̗v�f����ύX����B����ɂ��A�����I��ID���L�������
	mGlobalInvBindPoses.resize(GetNumBones());

	// �e�s��̋t�s������߂�
	for (size_t i = 0; i < mGlobalInvBindPoses.size(); i++)
	{
		mGlobalInvBindPoses[i] = mBones[i].mMatrix;
		mGlobalInvBindPoses[i].Invert();
	}
}