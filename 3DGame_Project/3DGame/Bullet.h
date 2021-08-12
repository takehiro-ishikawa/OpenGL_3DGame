#pragma once
#include "Actor.h"
#include "Character.h"

#define BULLET_LIFE 5.0f    // 消滅するまでの時間

#pragma region プロトタイプ宣言
class Game;
class MeshComponent;
class BulletMove;
class AudioComponent;
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