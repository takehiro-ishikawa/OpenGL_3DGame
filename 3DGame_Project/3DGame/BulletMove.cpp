#include "BulletMove.h"
#include "Actor.h"
#include "Character.h"
#include "Game.h"
#include "PhysWorld.h"
#include "TargetActor.h"
#include "Bullet.h"

BulletMove::BulletMove(Actor* owner)
	:MoveComponent(owner)
{

}

void BulletMove::Update(float deltaTime)
{
	// 進行方向に線分を構築する
	const float segmentLength = mMoveSpeed.y * deltaTime;
	Vector3 start = mOwner->GetPosition();
	Vector3 dir = mOwner->GetForward();
	Vector3 end = start + dir * segmentLength;
	LineSegment l(start, end);

	// 構築した線分と全てのボックスとで衝突判定を行う
	PhysWorld* phys = mOwner->GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;
	if (phys->SegmentCast(l, info) && info.mActor != mPlayer) // (プレイヤーとは衝突しない)
	{
		// 敵に当たった場合、ダメージを与える
		Character* target = dynamic_cast<Character*>(info.mActor);
		if (target)
		{
			target->Damage();
		}

		// 自身のアクターを消滅させる
		mOwner->SetState(Actor::ActorState::EDead);
	}

	// 基本クラスの更新は、前進速度に基づいて移動します
	MoveComponent::Update(deltaTime);
}