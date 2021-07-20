#include "AIState.h"
#include "StateMachine.h"
#include <SDL/SDL_log.h>
#include "Game.h"
#include "Actor.h"
#include "Enemy.h"
#include "SkeletalMeshComponent.h"


void AIIdle::Update(float deltaTime)
{

}

void AIIdle::OnEnter()
{
	mOwner->GetMeshComp()->PlayAnimation(mOwner->GetGame()->GetAnimation("Vigilant", "Assets/Enemy01.fbx"), 1.0f);
}

void AIIdle::OnExit()
{

}

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
	mOwner->GetMeshComp()->PlayAnimation(mOwner->GetGame()->GetAnimation("Die", "Assets/Enemy01.fbx"), 1.0f);
	mLifeSpan = ENEMY_DEADTIME;
}

void AIDead::OnExit()
{

}