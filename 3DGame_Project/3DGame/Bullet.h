#pragma once
#include "Actor.h"
#include "Character.h"
#include "MoveComponent.h"
#include <functional>

#define BULLET_LIFE 5.0f    // 消滅するまでの時間

#pragma region プロトタイプ宣言
class Game;
class MeshComponent;
class AudioComponent;
class BulletMove;
class PointLightComponent;
#pragma endregion

class Bullet : public Actor
{
public:
	Bullet(Game* game, Actor* bulletOwner, CharacterTag target, float damage);

	void UpdateActor(float deltaTime) override;
	MeshComponent* GetMeshComp() { return mMeshComp; }
	BulletMove* GetMoveComp() { return mMoveComp; }
	PointLightComponent* GetPointLightComp() { return mPointLight; }

	// 命中時に呼ぶ
	void HitTarget(Character* target);

private:
	MeshComponent* mMeshComp;
	BulletMove* mMoveComp;
	AudioComponent* mAudioComp;
	PointLightComponent* mPointLight;

	float mDamage;   // 命中時に対象に与えるダメージ
	float mLifeSpan; // 消滅するまでの時間
};


class BulletMove : public MoveComponent
{
public:
	BulletMove(Actor* owner, Actor* bulletOwner, CharacterTag targetTag, std::function<void(Character*)> onHit);

	void Update(float deltaTime) override;

	// ゲッター/セッター
	CharacterTag GetTargetTag() { return mTargetTag; }
	void SetTargetTag(CharacterTag tag) { mTargetTag = tag; }

protected:
	// 命中時に呼ばれるコールバック
	std::function<void(Character*)> mOnHit;

	Actor* mBulletOwner;     // 弾を発射したアクター
	CharacterTag mTargetTag; // 攻撃対象のタグ
};