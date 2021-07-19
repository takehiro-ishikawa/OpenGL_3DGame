#include "Skeleton.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL/SDL_log.h>
#include "MatrixPalette.h"
#include "FBXAnalyze.h"

bool Skeleton::Load(const std::string& fileName)
{
	FBXAnalyze* fbxFile = new FBXAnalyze(fileName.c_str());
	
	// �X�P���g���̃{�[���z����擾
	fbxFile->ComputeBindPoses(mBones);

	// �S�Ẵ{�[���̃O���[�o���ȋt�o�C���h�|�[�Y�s����v�Z����
	ComputeGlobalInvBindPose();

	// �������
	delete fbxFile;

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