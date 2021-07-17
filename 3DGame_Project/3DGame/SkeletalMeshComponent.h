#pragma once
#include "MeshComponent.h"
#include "MatrixPalette.h"

#pragma region �v���g�^�C�v�錾
class Actor;
class Shader;
class Skeleton;
class Animation;
#pragma endregion

class SkeletalMeshComponent : public MeshComponent
{
public:
	SkeletalMeshComponent(Actor* owner);
	
	// ���b�V���R���|�[�l���g��`��
	void Draw(Shader* shader) override;

	void Update(float deltaTime) override;

	// �Z�b�^�[
	void SetSkeleton(const Skeleton* sk) { mSkeleton = sk; }

	// �A�j���[�V�������Đ����B�A�j���[�V�����̒�����Ԃ�
	float PlayAnimation(const Animation* anim, float playRate = 1.0f);

protected:

	// �s��p���b�g���v�Z����
	void ComputeMatrixPalette();

	MatrixPalette mPalette;            // �s��p���b�g
	const Skeleton* mSkeleton;   // ���������蓖�Ă���X�P���g��
	const Animation* mAnimation; // ���Đ����Ă���A�j���[�V����
	float mAnimPlayRate;               // �A�j���[�V�����̍Đ����[�g�i1.0���ʏ�̃X�s�[�h�j
	float mAnimTime;                   // �A�j���[�V�����̌��݂̎���
};
