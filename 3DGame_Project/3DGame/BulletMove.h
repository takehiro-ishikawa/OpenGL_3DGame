#pragma once
#include "MoveComponent.h"
#include "Character.h"

#pragma region プロトタイプ宣言
class Actor;
#pragma endregion

class BulletMove : public MoveComponent
{
public:
	BulletMove(Actor* owner, CharacterTag targetTag);

	void Update(float deltaTime) override;

	// ゲッター/セッター
	CharacterTag GetTargetTag() { return mTargetTag; }
	void SetTargetTag(CharacterTag tag) { mTargetTag = tag; }
	void SetPlayer(Actor* player) { mPlayer = player; }

protected:
	Actor* mPlayer;
	CharacterTag mTargetTag;
};