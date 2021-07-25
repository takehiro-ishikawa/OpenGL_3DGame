#pragma once
#include "Actor.h"

#define BULLET_FILEPATH "Assets/Bullet.fbx"
#define BULLET_LIFE 10.0f    // 消滅するまでの時間
#define BULLET_SPEED 5000.0f // 速度

#pragma region プロトタイプ宣言
class Game;
class BulletMove;
class AudioComponent;
class PointLightComponent;
#pragma endregion

class Bullet : public Actor
{
public:
	Bullet(Game* game);

	void UpdateActor(float deltaTime) override;
	void SetPlayer(Actor* player);

	void HitTarget();

private:
	BulletMove* mMoveComp;
	AudioComponent* mAudioComp;
	PointLightComponent* mPointLight;

	// 消滅するまでの時間
	float mLifeSpan;
};