#include "SkeletalMeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Animation.h"
#include "Skeleton.h"
#include <iostream>

SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner)
	:MeshComponent(owner, true)
	, mAnimation(nullptr)
	, mSkeleton(nullptr)
	, mAnimTime(0.0f)
	, mAnimPlayRate(1.0f)
{
}

void SkeletalMeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		// ���[���h���W��ݒ肷��
		shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());

		// �s��p���b�g��ݒ肷��
		shader->SetMatrixUniforms("uMatrixPalette", &mPalette.mEntry[0], MAX_SKELETON_BONES);

		// ���ʔ��˒l��ݒ肷��
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());

		// �A�N�e�B�u�ȃe�N�X�`����ݒ肷��
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}

		// ���b�V���̒��_�z����A�N�e�B�u�Ƃ��Đݒ肷��
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();

		// �`�悷��
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

void SkeletalMeshComponent::Update(float deltaTime)
{
	if (mAnimation && mSkeleton)
	{
		mAnimTime += deltaTime * mAnimPlayRate;

		// �����𒴂����犪���߂�
		while (mAnimTime > mAnimation->GetDuration())
		{
			mAnimTime -= mAnimation->GetDuration();
		}

		// �s��p���b�g���Čv�Z����
		ComputeMatrixPalette();
	}
}

float SkeletalMeshComponent::PlayAnimation(const Animation* anim, float playRate)
{
	// �V�����A�j���[�V���������蓖�Ă�
	mAnimation = anim;
	mAnimTime = 0.0f;
	mAnimPlayRate = playRate;

	if (!mAnimation) { return 0.0f; }

	// �s��p���b�g���v�Z
	ComputeMatrixPalette();

	// �A�j���[�V�����̒�����Ԃ�
	return mAnimation->GetDuration();
}

void SkeletalMeshComponent::ComputeMatrixPalette()
{
	// �{�[���̃O���[�o���ȋt�o�C���h�|�[�Y�s����擾
	const std::vector<Matrix4>& globalInvBindPoses = mSkeleton->GetGlobalInvBindPoses();

	// �w�肵�������ɂ�����e�{�[���̃O���[�o���|�[�Y�s����擾
	std::vector<Matrix4> currentPoses;
	mAnimation->GetGlobalPoseAtTime(currentPoses, mSkeleton, mAnimTime);
	
	// �e�{�[���̃p���b�g��ݒ肷��
	for (size_t i = 0; i < mSkeleton->GetNumBones(); i++)
	{
		// �O���[�o���ȋt�o�C���h�|�[�Y�s��ƌ��݂̃|�[�Y�s��̐�
		mPalette.mEntry[i] = globalInvBindPoses[i] * currentPoses[i];
	}
}