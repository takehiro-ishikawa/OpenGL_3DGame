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
#include "PhysWorld.h"
#include "Player.h"
#include "Bullet.h"
#include "PointLightComponent.h"
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
	bullet->GetPointLightComp()->mDiffuseColor = Color::Red;
	bullet->GetPointLightComp()->mOuterRadius = 200.0f;
	bullet->GetPointLightComp()->mInnerRadius = 100.0f;
	bullet->SetPlayer(this);
	bullet->SetPosition(start);
	// ボールを回転させて新しい方向を向く
	bullet->RotateToNewForward(GetForward());

	mAudioComp->PlayEvent("event:/Shoot");
}

bool Enemy::CheckPlayerVisible()
{
	// 線分の作成
	Vector3 start = GetCenterPosition();
	Vector3 end = start + GetForward() * ENEMY_VISIBLE_RANGE;

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