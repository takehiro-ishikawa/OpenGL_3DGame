#pragma once
#include "MoveComponent.h"
#include "Character.h"
#include <functional>

#pragma region プロトタイプ宣言
class Actor;
#pragma endregion

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