#pragma once
#include "Actor.h"

#define BULLET_FILEPATH "Assets/Bullet.fbx"
#define BULLET_LIFE 10.0f    // ���ł���܂ł̎���
#define BULLET_SPEED 5000.0f // ���x

#pragma region �v���g�^�C�v�錾
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

	// ���ł���܂ł̎���
	float mLifeSpan;
};