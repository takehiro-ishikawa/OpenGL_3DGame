#include "AIState.h"
#include "StateMachine.h"
#include <SDL/SDL_log.h>
#include "Game.h"
#include "Actor.h"
#include "Enemy.h"
#include "SkeletalMeshComponent.h"
#include "MoveComponent.h"
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
	mEnemy->GetMoveComp()->SetAngularSpeed(0);
	mEnemy->GetMeshComp()->PlayAnimation(mEnemy->GetGame()->GetAnimation(ENEMY_ANIMATION_DEAD, ENEMY_FILEPATH), 1.0f);
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
			mIsRotate = true;
		}
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
	mIsRotate = false;
	mRestTime = ENEMY_RESTTIME;
	mNextDir = mEnemy->GetRight();
}

#pragma endregion
