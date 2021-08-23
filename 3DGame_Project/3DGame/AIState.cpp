#include "StateMachine.h"
#include <SDL/SDL_log.h>
#include "Game.h"
#include "Actor.h"
#include "Enemy.h"
#include "Player.h"
#include "SkeletalAnimation.h"
#include "MoveComponent.h"
#include "Audio.h"
#include <iostream>

AIState::AIState(Character* owner)
	:State(owner)
{
	mEnemy = dynamic_cast<Enemy*>(owner);

	if (mEnemy == nullptr)
	{
		SDL_Log("Failed to get Enemy pointer");
	}
}

#pragma region "�ҋ@"���

void AIIdle::Update(float deltaTime)
{

}

void AIIdle::OnEnter()
{
	mOwner->GetMeshComp()->PlayAnimation(mOwner->GetGame()->GetAnimation(ENEMY_ANIMATION_IDLE, ENEMY_FILEPATH), 1.0f);
}

void AIIdle::OnExit()
{

}

#pragma endregion


#pragma region "���S"���

void AIDead::Update(float deltaTime)
{
	mLifeSpan -= deltaTime;
	if (mLifeSpan < 0)
	{
		mOwner->SetState(Actor::ActorState::EDead);
	}
}

void AIDead::OnEnter()
{
	// SE�Đ�
	mEnemy->GetAudioComp()->PlayEvent(SE_ENEMY_DOWN);
	// �����͎~�߂�
	mEnemy->PlayFootStep(false);

	// �������~�߂�
	mEnemy->GetMoveComp()->SetAngularSpeed(0);
	mEnemy->GetMoveComp()->SetMoveSpeed(Vector2::Zero);

	// ���S�A�j���[�V����
	mEnemy->GetMeshComp()->PlayAnimation(mEnemy->GetGame()->GetAnimation(ENEMY_ANIMATION_DEAD, ENEMY_FILEPATH), 1.0f);

	// (ENEMY_DEADTIME)�b��ɏ�����
	mLifeSpan = ENEMY_DEADTIME;
}

void AIDead::OnExit()
{

}

#pragma endregion


#pragma region "�x��"���

void AIVigilant::Update(float deltaTime)
{
	// ��]��
	if (mIsRotate)
	{
		// mNextDir�̕����������I����Ă���Ȃ��莞�ԐÎ~����
		float angular = Vector3::Dot(mEnemy->GetRight(), mNextDir);
		if (Math::NearZero(angular)) Rest();

		// �v���C���[�̉�]
		float angularSpeed = angular * ENEMY_ROTATE_SPEED;
		angularSpeed = Math::Min(angularSpeed, ENEMY_MAX_ROTATE_SPEED);
		mEnemy->GetMoveComp()->SetAngularSpeed(angularSpeed);
	}
	// �Î~��
	else
	{
		// ��莞�Ԃ��߂�����Ăщ�]���J�n����
		mRestTime -= deltaTime;
		if (mRestTime < 0)
		{
			mRestTime = 0;
			mOwner->GetMeshComp()->PlayAnimation(mOwner->GetGame()->GetAnimation(ENEMY_ANIMATION_MOVE, ENEMY_FILEPATH), 1.0f);
			mEnemy->PlayFootStep(true);
			mIsRotate = true;
		}
	}

	// ���E�Ƀv���C���[���Ƃ炦��
	if (mEnemy->CheckInViewPlayer(ENEMY_VISIBLE_ANGLE, ENEMY_VISIBLE_RANGE) && mEnemy->CheckInterruptObject())
	{
		// "�U��"��Ԃ֑J��
		mEnemy->ChangeState(AI_ATTACK);
	}
}

void AIVigilant::OnEnter()
{
	Rest();
}

void AIVigilant::OnExit()
{

}

void AIVigilant::Rest()
{
	mOwner->GetMeshComp()->PlayAnimation(mOwner->GetGame()->GetAnimation(ENEMY_ANIMATION_IDLE, ENEMY_FILEPATH), 1.0f);
	mEnemy->PlayFootStep(false);
	mIsRotate = false;
	mRestTime = ENEMY_RESTTIME;
	mNextDir = mEnemy->GetRight();
}

#pragma endregion


#pragma region "�U��"���

void AIAttack::Update(float deltaTime)
{
	// �v���C���[�̕���������
	Vector3 playerDir = mEnemy->GetGame()->GetPlayer()->GetPosition() - mEnemy->GetPosition();
	playerDir.z = 0;
	playerDir.Normalize();
	float angular = Vector3::Dot(mEnemy->GetRight(), playerDir);
	mEnemy->GetMoveComp()->SetAngularSpeed(angular * ENEMY_ROTATE_SPEED);

	// �ړ�
	mEnemy->GetMoveComp()->SetMoveSpeed(Vector2(0, ENEMY_MOVESPEED));

	// �U��
	mAttackSpan -= deltaTime;
	if (mAttackSpan <= 0)
	{
		mAttackSpan = ENEMY_ATTACK_TIME;
		mEnemy->Shoot();
	}
}

void AIAttack::OnEnter()
{
	// ����SE�Đ�
	mEnemy->GetAudioComp()->PlayEvent(SE_ENEMY_DISCOVER);
	mAttackSpan = ENEMY_ATTACK_TIME;
	mOwner->GetMeshComp()->PlayAnimation(mOwner->GetGame()->GetAnimation(ENEMY_ANIMATION_MOVE, ENEMY_FILEPATH), 1.0f);
	mEnemy->PlayFootStep(true);
}

void AIAttack::OnExit()
{
	mEnemy->PlayFootStep(false);
}

#pragma endregion