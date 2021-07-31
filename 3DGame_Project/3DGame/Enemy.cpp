#include "Enemy.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "SkeletalMeshComponent.h"
#include "BoxComponent.h"
#include "AudioComponent.h"
#include "SoundEvent.h"
#include "MoveComponent.h"
#include "BulletMove.h"
#include "StateMachine.h"
#include "AIState.h"
#include "PhysWorld.h"
#include "Player.h"
#include "Bullet.h"
#include "PointLightComponent.h"
#include <typeinfo>
#include <iostream>


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
	game->GetRenderer()->GetMesh(ENEMYBULLET_FILEPATH);

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
	RegisterState(new AIAttack(this));
	ChangeState(AI_VIGILANT);

	// 体力の設定
	mHealth = ENEMY_MAX_HEALTH;

	// タグの設定
	mTag = CharacterTag::EEnemy;

	// 自身の中心座標の設定
	mCenterOffset = ENEMY_CENTER_OFFSET;

	// 足音SEの割り当て
	mFootStep = mAudioComp->PlayEvent(SE_FOOTSTEP_ENEMY);
	mFootStep.SetPaused(true);
}

void Enemy::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	FixCollisions();

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

void Enemy::Shoot()
{
	// 開始点を取得
	Vector3 start = GetPosition();
	Vector3 offset = ENEMY_SHOOT_POSITION_OFFSET;
	start += GetRight() * offset.x;
	start += GetForward() * offset.y;
	start.z += offset.z;

	// ボールをスポーンする
	Bullet* bullet = new Bullet(GetGame(), CharacterTag::EPlayer);
	bullet->GetMeshComp()->SetMesh(GetGame()->GetRenderer()->GetMesh(ENEMYBULLET_FILEPATH));
	bullet->GetMoveComp()->SetMoveSpeed(Vector2(0, ENEMY_BULLET_SPEED));
	bullet->GetPointLightComp()->mDiffuseColor = Color::Red;
	bullet->GetPointLightComp()->mOuterRadius = 200.0f;
	bullet->GetPointLightComp()->mInnerRadius = 100.0f;
	bullet->SetPlayer(this);
	bullet->SetPosition(start);
	// ボールを回転させて新しい方向を向く
	bullet->RotateToNewForward(GetForward());

	// SEを鳴らす
	mAudioComp->PlayEvent(SE_SHOOT_E);
}

bool Enemy::CheckPlayerVisible(float angle, float length)
{
	// 視野内にいるか
	Vector3 playerDir = GetGame()->GetPlayer()->GetCenterPosition() - GetCenterPosition();
	playerDir.Normalize();
	float rad = Math::Acos(Vector3::Dot(GetForward(), playerDir));
    if(Math::ToDegrees(rad) >= angle / 2) return false; // いないならfalseを返す

	// 障害物の無い直線上の指定範囲内にプレイヤーがいるか
	Vector3 start = GetCenterPosition();
	Vector3 end = start + playerDir * length;
	LineSegment segment(start, end);
	PhysWorld* phys = GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;
	// 視線の先に何かあるか？
	if (phys->SegmentCast(segment, info, this))
	{
		Player* player = dynamic_cast<Player*>(info.mActor);
		// それはプレイヤーか？
		if (player) return true;
		else return false;
	}
	else
	{
		return false;
	}
}

void Enemy::PlayFootStep(bool isPlay)
{
	// 再生
	if (isPlay)
	{
		if (!mFootStep.GetPaused()) return;

		mFootStep.SetPaused(!isPlay);
		mFootStep.Restart();
	}
	// 停止
	else
	{
		mFootStep.SetPaused(!isPlay);
	}
}