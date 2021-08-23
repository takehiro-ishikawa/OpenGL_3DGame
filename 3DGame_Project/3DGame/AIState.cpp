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

#pragma region "‘Ò‹@"ó‘Ô

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


#pragma region "€–S"ó‘Ô

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
	// SEÄ¶
	mEnemy->GetAudioComp()->PlayEvent(SE_ENEMY_DOWN);
	// ‘«‰¹‚Í~‚ß‚é
	mEnemy->PlayFootStep(false);

	// “®‚«‚ğ~‚ß‚é
	mEnemy->GetMoveComp()->SetAngularSpeed(0);
	mEnemy->GetMoveComp()->SetMoveSpeed(Vector2::Zero);

	// €–SƒAƒjƒ[ƒVƒ‡ƒ“
	mEnemy->GetMeshComp()->PlayAnimation(mEnemy->GetGame()->GetAnimation(ENEMY_ANIMATION_DEAD, ENEMY_FILEPATH), 1.0f);

	// (ENEMY_DEADTIME)•bŒã‚ÉÁ‚¦‚é
	mLifeSpan = ENEMY_DEADTIME;
}

void AIDead::OnExit()
{

}

#pragma endregion


#pragma region "Œx‰ú"ó‘Ô

void AIVigilant::Update(float deltaTime)
{
	// ‰ñ“]’†
	if (mIsRotate)
	{
		// mNextDir‚Ì•ûŒü‚ğŒü‚«I‚í‚Á‚Ä‚¢‚é‚È‚çˆê’èŠÔÃ~‚·‚é
		float angular = Vector3::Dot(mEnemy->GetRight(), mNextDir);
		if (Math::NearZero(angular)) Rest();

		// ƒvƒŒƒCƒ„[‚Ì‰ñ“]
		float angularSpeed = angular * ENEMY_ROTATE_SPEED;
		angularSpeed = Math::Min(angularSpeed, ENEMY_MAX_ROTATE_SPEED);
		mEnemy->GetMoveComp()->SetAngularSpeed(angularSpeed);
	}
	// Ã~’†
	else
	{
		// ˆê’èŠÔ‚ª‰ß‚¬‚½‚çÄ‚Ñ‰ñ“]‚ğŠJn‚·‚é
		mRestTime -= deltaTime;
		if (mRestTime < 0)
		{
			mRestTime = 0;
			mOwner->GetMeshComp()->PlayAnimation(mOwner->GetGame()->GetAnimation(ENEMY_ANIMATION_MOVE, ENEMY_FILEPATH), 1.0f);
			mEnemy->PlayFootStep(true);
			mIsRotate = true;
		}
	}

	// ‹ŠE‚ÉƒvƒŒƒCƒ„[‚ğ‚Æ‚ç‚¦‚½
	if (mEnemy->CheckInViewPlayer(ENEMY_VISIBLE_ANGLE, ENEMY_VISIBLE_RANGE) && mEnemy->CheckInterruptObject())
	{
		// "UŒ‚"ó‘Ô‚Ö‘JˆÚ
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


#pragma region "UŒ‚"ó‘Ô

void AIAttack::Update(float deltaTime)
{
	// ƒvƒŒƒCƒ„[‚Ì•ûŒü‚ğŒü‚­
	Vector3 playerDir = mEnemy->GetGame()->GetPlayer()->GetPosition() - mEnemy->GetPosition();
	playerDir.z = 0;
	playerDir.Normalize();
	float angular = Vector3::Dot(mEnemy->GetRight(), playerDir);
	mEnemy->GetMoveComp()->SetAngularSpeed(angular * ENEMY_ROTATE_SPEED);

	// ˆÚ“®
	mEnemy->GetMoveComp()->SetMoveSpeed(Vector2(0, ENEMY_MOVESPEED));

	// UŒ‚
	mAttackSpan -= deltaTime;
	if (mAttackSpan <= 0)
	{
		mAttackSpan = ENEMY_ATTACK_TIME;
		mEnemy->Shoot();
	}
}

void AIAttack::OnEnter()
{
	// ”­Œ©SEÄ¶
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