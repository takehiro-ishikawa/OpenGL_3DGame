#include "Enemy.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "SkeletalMeshComponent.h"
#include "BoxComponent.h"
#include "AudioComponent.h"
#include "MoveComponent.h"
#include "StateMachine.h"
#include "AIState.h"
#include <typeinfo>


Enemy::Enemy(Game* game)
	:Character(game)
{
	SetScale(ENEMY_SCALE);
	SetPosition(GetPosition());

	// コンポーネントの生成
	mMoveComp = new MoveComponent(this);

	// メッシュの生成
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh(ENEMY_FILEPATH));
	mMeshComp->SetSkeleton(game->GetSkeleton(ENEMY_FILEPATH));

	// アニメーションのロード
	mMeshComp->PlayAnimation(game->GetAnimation(ENEMY_ANIMATION_DEAD, ENEMY_FILEPATH), 1.0f);
	mMeshComp->PlayAnimation(game->GetAnimation(ENEMY_ANIMATION_MOVE, ENEMY_FILEPATH), 1.0f);
	mMeshComp->PlayAnimation(game->GetAnimation(ENEMY_ANIMATION_IDLE, ENEMY_FILEPATH), 1.0f);

	// AABBの設定
	AABB box = mMeshComp->GetMesh()->GetBox();
	box.SetMinMax(ENEMY_WIDTH, ENEMY_DEPTH, ENEMY_HEIGHT);
	mBoxComp->SetObjectBox(box);
	mBoxComp->SetShouldRotate(false);

	// AIステートの設定
	RegisterState(new AIIdle(this));
	RegisterState(new AIDead(this));
	RegisterState(new AIVigilant(this));
	ChangeState(AI_VIGILANT);

	// 体力の設定
	mHealth = ENEMY_MAX_HEALTH;
}

void Enemy::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	if (mCurrentState)
	{
		mCurrentState->Update(deltaTime);
	}
}

void Enemy::Damage(float value)
{
	if (typeid(*mCurrentState) != typeid(AIDead))
	{
		mHealth -= value;
		mHealth = Math::Max(mHealth, 0.0f);
		mHealth = Math::Min(mHealth, ENEMY_MAX_HEALTH);

		if (mHealth <= 0)
		{
			Dead();
		}
	}
}

void Enemy::Dead()
{
	ChangeState(AI_DEAD);
}