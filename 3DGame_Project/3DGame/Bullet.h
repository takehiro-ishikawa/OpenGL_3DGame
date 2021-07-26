#pragma once
#include "Actor.h"
#include "Character.h"

#define BULLET_LIFE 10.0f    // ���ł���܂ł̎���
#define BULLET_SPEED 5000.0f // ���x

#pragma region �v���g�^�C�v�錾
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

	// ���ł���܂ł̎���
	float mLifeSpan;
};