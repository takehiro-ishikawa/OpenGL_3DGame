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
		// ワールド座標を設定する
		shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());

		// 行列パレットを設定する
		shader->SetMatrixUniforms("uMatrixPalette", &mPalette.mEntry[0], MAX_SKELETON_BONES);

		// 鏡面反射値を設定する
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());

		// アクティブなテクスチャを設定する
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}

		// メッシュの頂点配列をアクティブとして設定する
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();

		// 描画する
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

void SkeletalMeshComponent::Update(float deltaTime)
{
	if (mAnimation && mSkeleton)
	{
		mAnimTime += deltaTime * mAnimPlayRate;

		// 長さを超えたら巻き戻す
		while (mAnimTime > mAnimation->GetDuration())
		{
			mAnimTime -= mAnimation->GetDuration();
		}

		// 行列パレットを再計算する
		ComputeMatrixPalette();
	}
}

float SkeletalMeshComponent::PlayAnimation(const Animation* anim, float playRate)
{
	// 新しいアニメーションを割り当てる
	mAnimation = anim;
	mAnimTime = 0.0f;
	mAnimPlayRate = playRate;

	if (!mAnimation) { return 0.0f; }

	// 行列パレットを計算
	ComputeMatrixPalette();

	// アニメーションの長さを返す
	return mAnimation->GetDuration();
}

void SkeletalMeshComponent::ComputeMatrixPalette()
{
	// ボーンのグローバルな逆バインドポーズ行列を取得
	const std::vector<Matrix4>& globalInvBindPoses = mSkeleton->GetGlobalInvBindPoses();

	// 指定した時刻における各ボーンのグローバルポーズ行列を取得
	std::vector<Matrix4> currentPoses;
	mAnimation->GetGlobalPoseAtTime(currentPoses, mSkeleton, mAnimTime);
	
	// 各ボーンのパレットを設定する
	for (size_t i = 0; i < mSkeleton->GetNumBones(); i++)
	{
		// グローバルな逆バインドポーズ行列と現在のポーズ行列の積
		mPalette.mEntry[i] = globalInvBindPoses[i] * currentPoses[i];
	}
}