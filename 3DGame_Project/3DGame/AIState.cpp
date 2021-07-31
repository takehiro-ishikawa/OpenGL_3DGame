#include "AIState.h"
#include "StateMachine.h"
#include <SDL/SDL_log.h>
#include "Game.h"
#include "Actor.h"
#include "Enemy.h"
#include "Player.h"
#include "SkeletalMeshComponent.h"
#include "MoveComponent.h"
#include "AudioComponent.h"
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

#pragma region "待機"状態

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

#pragma region "死亡"状態

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
	// SE再生
	mEnemy->GetAudioComp()->PlayEvent(SE_ENEMY_DOWN);
	// 足音は止める
	mEnemy->PlayFootStep(false);

	// 動きを止める
	mEnemy->GetMoveComp()->SetAngularSpeed(0);
	mEnemy->GetMoveComp()->SetMoveSpeed(Vector2::Zero);

	// 死亡アニメーション
	mEnemy->GetMeshComp()->PlayAnimation(mEnemy->GetGame()->GetAnimation(ENEMY_ANIMATION_DEAD, ENEMY_FILEPATH), 1.0f);

	// (ENEMY_DEADTIME)秒後に消える
	mLifeSpan = ENEMY_DEADTIME;
}

void AIDead::OnExit()
{

}

#pragma endregion

#pragma region "警戒"状態

void AIVigilant::Update(float deltaTime)
{
	// 視界にプレイヤーをとらえた
	if (mEnemy->CheckPlayerVisible(ENEMY_VISIBLE_ANGLE, ENEMY_VISIBLE_RANGE))
	{
		mEnemy->GetAudioComp()->PlayEvent(SE_ENEMY_DISCOVER);
		mEnemy->ChangeState(AI_ATTACK);
	}

	// 回転中
	if (mIsRotate)
	{
		// mNextDirの方向を向き終わっているなら一定時間静止する
		float angular = Vector3::Dot(mEnemy->GetRight(), mNextDir);
		if (Math::NearZero(angular)) Rest();

		// プレイヤーの回転
		float angularSpeed = angular * ENEMY_ROTATE_SPEED;
		angularSpeed = Math::Min(angularSpeed, ENEMY_MAX_ROTATE_SPEED);
		mEnemy->GetMoveComp()->SetAngularSpeed(angularSpeed);
	}
	// 静止中
	else
	{
		// 一定時間が過ぎたら再び回転を開始する
		mRestTime -= deltaTime;
		if (mRestTime < 0)
		{
			mRestTime = 0;
			mOwner->GetMeshComp()->PlayAnimation(mOwner->GetGame()->GetAnimation(ENEMY_ANIMATION_MOVE, ENEMY_FILEPATH), 1.0f);
			mEnemy->PlayFootStep(true);
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
	mEnemy->PlayFootStep(false);
	mIsRotate = false;
	mRestTime = ENEMY_RESTTIME;
	mNextDir = mEnemy->GetRight();
}

#pragma endregion

#pragma region "攻撃"状態

void AIAttack::Update(float deltaTime)
{
	// プレイヤーの方向を向く
	Vector3 playerDir = mEnemy->GetGame()->GetPlayer()->GetPosition() - mEnemy->GetPosition();
	playerDir.z = 0;
	playerDir.Normalize();
	float angular = Vector3::Dot(mEnemy->GetRight(), playerDir);
	mEnemy->GetMoveComp()->SetAngularSpeed(angular * ENEMY_ROTATE_SPEED);

	mEnemy->CheckPlayerVisible(ENEMY_VISIBLE_ANGLE, ENEMY_VISIBLE_RANGE);

	mAttackSpan -= deltaTime;
	if (mAttackSpan <= 0)
	{
		mAttackSpan = ENEMY_ATTACK_TIME;
		mEnemy->Shoot();
	}
}

void AIAttack::OnEnter()
{
	mAttackSpan = ENEMY_ATTACK_TIME;
	mOwner->GetMeshComp()->PlayAnimation(mOwner->GetGame()->GetAnimation(ENEMY_ANIMATION_IDLE, ENEMY_FILEPATH), 1.0f);
	mEnemy->PlayFootStep(false);
}

void AIAttack::OnExit()
{

}

#pragma endregion

#pragma region "追跡"状態

void AIChase::Update(float deltaTime)
{
	// プレイヤーの方向を向く
	Vector3 playerDir = mEnemy->GetGame()->GetPlayer()->GetPosition() - mEnemy->GetPosition();
	playerDir.z = 0;
	playerDir.Normalize();
	float angular = Vector3::Dot(mEnemy->GetRight(), playerDir);
	mEnemy->GetMoveComp()->SetAngularSpeed(angular * ENEMY_ROTATE_SPEED);

	// プレイヤーを補足しているか
	//if(mEnemy->CheckPlayerVisible(ENEMY_VISIBLE_RANGE))
}

void AIChase::OnEnter()
{
	// 移動速度設定
	mEnemy->GetMoveComp()->SetMoveSpeed(Vector2(0, ENEMY_MOVESPEED));
}

void AIChase::OnExit()
{
	// 停止
	mEnemy->GetMoveComp()->SetMoveSpeed(Vector2::Zero);
}

#pragma endregion

