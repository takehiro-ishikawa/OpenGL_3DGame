#include "Enemy.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "SkeletalMeshComponent.h"
#include "BoxComponent.h"
#include "AudioComponent.h"
#include "StateMachine.h"
#include "AIState.h"
#include <typeinfo>


Enemy::Enemy(Game* game)
	:Character(game)
{
	SetScale(120.0f);
	SetPosition(GetPosition());

	// ���b�V���̐���
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh(ENEMY_FILEPATH));
	mMeshComp->SetSkeleton(game->GetSkeleton(ENEMY_FILEPATH));

	// �A�j���[�V�����̃��[�h
	mMeshComp->PlayAnimation(game->GetAnimation(ENEMY_ANIMATION_DEAD, ENEMY_FILEPATH), 1.0f);
	mMeshComp->PlayAnimation(game->GetAnimation(ENEMY_ANIMATION_VIGILANT, ENEMY_FILEPATH), 1.0f);

	// AABB�̐ݒ�
	AABB box = mMeshComp->GetMesh()->GetBox();
	box.SetMinMax(ENEMY_WIDTH, ENEMY_DEPTH, ENEMY_HEIGHT);
	mBoxComp->SetObjectBox(box);
	mBoxComp->SetShouldRotate(false);

	// AI�X�e�[�g�̐ݒ�
	RegisterState(new AIIdle(this));
	RegisterState(new AIDead(this));
	ChangeState(AI_IDLE);
}

void Enemy::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	if (mCurrentState)
	{
		mCurrentState->Update(deltaTime);
	}
}

void Enemy::Damage()
{
	if (typeid(*mCurrentState) != typeid(AIDead))
	{
		ChangeState(AI_DEAD);
	}
}