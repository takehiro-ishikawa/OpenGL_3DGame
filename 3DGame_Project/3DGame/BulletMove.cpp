#include "BulletMove.h"
#include "Actor.h"
#include "Character.h"
#include "Game.h"
#include "PhysWorld.h"
#include "TargetActor.h"
#include "Bullet.h"

BulletMove::BulletMove(Actor* owner, Actor* bulletOwner, CharacterTag targetTag, std::function<void(Character*)> onHit)
	:MoveComponent(owner)
	,mBulletOwner(bulletOwner)
	,mTargetTag(targetTag)
    ,mOnHit(onHit)
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
	if (phys->SegmentCast(l, info) && info.mActor != mBulletOwner) // (弾を発射したアクターとは衝突しない)
	{
		Character* target = dynamic_cast<Character*>(info.mActor);
		// キャラクターに当たった場合
		if (target)
		{
			// 当たったキャラクターが自身の標的ならダメージを与える
			if(target->GetCharacterTag() == mTargetTag) mOnHit(target);
		}

		// 自身を消滅させる
		mOwner->SetState(Actor::ActorState::EDead);
	}

	// 基本クラスの更新は、前進速度に基づいて移動します
	MoveComponent::Update(deltaTime);
}