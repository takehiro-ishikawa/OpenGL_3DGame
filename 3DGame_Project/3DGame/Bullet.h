#pragma once
#include "Actor.h"
#include "Character.h"

#define BULLET_LIFE 5.0f    // ���ł���܂ł̎���

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
	Bullet(Game* game, Actor* bulletOwner, CharacterTag target, float damage);

	void UpdateActor(float deltaTime) override;
	MeshComponent* GetMeshComp() { return mMeshComp; }
	BulletMove* GetMoveComp() { return mMoveComp; }
	PointLightComponent* GetPointLightComp() { return mPointLight; }

	// �������ɌĂ�
	void HitTarget(Character* target);

private:
	MeshComponent* mMeshComp;
	BulletMove* mMoveComp;
	AudioComponent* mAudioComp;
	PointLightComponent* mPointLight;

	float mDamage;   // �������ɑΏۂɗ^����_���[�W
	float mLifeSpan; // ���ł���܂ł̎���
};