#pragma once
#include "Actor.h"
#include "Character.h"

#define BULLET_LIFE 10.0f    // 消滅するまでの時間
#define BULLET_SPEED 5000.0f // 速度

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
	Bullet(Game* game, CharacterTag target);

	void UpdateActor(float deltaTime) override;
	void SetPlayer(Actor* player);
	MeshComponent* GetMeshComp() { return mMeshComp; }
	PointLightComponent* GetPointLightComp() { return mPointLight; }

	void HitTarget();

private:
	MeshComponent* mMeshComp;
	BulletMove* mMoveComp;
	AudioComponent* mAudioComp;
	PointLightComponent* mPointLight;

	// 消滅するまでの時間
	float mLifeSpan;
};