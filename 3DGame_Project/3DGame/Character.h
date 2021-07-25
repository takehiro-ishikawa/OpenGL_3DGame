#pragma once
#include "Actor.h"

#pragma region プロトタイプ宣言
class Game;
class SkeletalMeshComponent;
class BoxComponent;
class AudioComponent;
#pragma endregion

class Character : public Actor
{
public:
	Character(Game* game);

	virtual void Damage(float damage) {};
	virtual void Dead() {};

	// ゲッター
	SkeletalMeshComponent* GetMeshComp() { return mMeshComp; }
	BoxComponent* GetBoxComp() { return mBoxComp; }
	AudioComponent* GetAudioComp() { return mAudioComp; }
	float GetHealth() { return mHealth; }
	void SetHealth(float value) { mHealth = value; }

protected:

	// 着地判定
	bool CheckGround();

	// 「最小の重なり」で衝突をテストする
	void FixCollisions();

	SkeletalMeshComponent* mMeshComp;
	BoxComponent* mBoxComp;
	AudioComponent* mAudioComp;

	// 体力
	float mHealth;
};