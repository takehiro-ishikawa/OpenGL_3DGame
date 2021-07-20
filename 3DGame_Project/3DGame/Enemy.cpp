#include "Enemy.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "SkeletalMeshComponent.h"
#include "BoxComponent.h"
#include "AudioComponent.h"
#include "StateMachine.h"
#include "AIState.h"


Enemy::Enemy(Game* game)
	:Character(game)
{
	SetScale(120.0f);
	SetPosition(GetPosition());

	// メッシュの生成
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/Enemy01.fbx"));
	mMeshComp->SetSkeleton(game->GetSkeleton("Assets/Enemy01.fbx"));

	// アニメーションのロード
	mMeshComp->PlayAnimation(game->GetAnimation("Die", "Assets/Enemy01.fbx"), 1.0f);
	mMeshComp->PlayAnimation(game->GetAnimation("Vigilant", "Assets/Enemy01.fbx"), 1.0f);

	// AABBの設定
	AABB box = mMeshComp->GetMesh()->GetBox();
	box.SetMinMax(ENEMY_WIDTH, ENEMY_DEPTH, ENEMY_HEIGHT);
	mBoxComp->SetObjectBox(box);
	mBoxComp->SetShouldRotate(false);

	// AIステートの設定
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
	ChangeState(AI_DEAD);
}