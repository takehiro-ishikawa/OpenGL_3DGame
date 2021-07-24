#pragma once
#include "Actor.h"

#define BULLET_FILEPATH "Assets/Bullet.fbx"
#define BULLET_LIFE 5.0f     // ���ł���܂ł̎���
#define BULLET_SPEED 5000.0f // ���x

#pragma region �v���g�^�C�v�錾
class Game;
class BulletMove;
class AudioComponent;
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

	// ���ł���܂ł̎���
	float mLifeSpan;
};