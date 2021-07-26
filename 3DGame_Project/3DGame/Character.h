#pragma once
#include "Actor.h"

#define CHECKGROUND_RENGE  2.0f // 着地判定時の線分の長さ
#define CHECKGROUND_OFFSET 1.0f // 着地判定時の線分の開始位置のZ方向のオフセット値

#pragma region プロトタイプ宣言
class Game;
class SkeletalMeshComponent;
class BoxComponent;
class AudioComponent;
#pragma endregion

enum class CharacterTag
{
	EPlayer, // プレイヤー
	EEnemy,  // 敵キャラ
	ENone    // その他
};

class Character : public Actor
{
public:
	Character(Game* game);

	virtual void Damage(float damage) {};
	virtual void Dead() {};

	// ゲッター
	CharacterTag GetCharacterTag() { return mTag; }
	SkeletalMeshComponent* GetMeshComp() { return mMeshComp; }
	BoxComponent* GetBoxComp() { return mBoxComp; }
	AudioComponent* GetAudioComp() { return mAudioComp; }
	float GetHealth() { return mHealth; }
	void SetCharacterTag(CharacterTag tag) { mTag = tag; }
	void SetHealth(float value) { mHealth = value; }

protected:

	// 着地判定
	bool CheckGround();

	// 「最小の重なり」で衝突をテストする
	void FixCollisions();

	SkeletalMeshComponent* mMeshComp;
	BoxComponent* mBoxComp;
	AudioComponent* mAudioComp;

	// 自身のキャラクタータグ
	CharacterTag mTag;

	// 体力
	float mHealth;
};