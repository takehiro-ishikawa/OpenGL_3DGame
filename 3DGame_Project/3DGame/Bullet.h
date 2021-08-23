#pragma once
#include "Actor.h"
#include "Character.h"
#include "MoveComponent.h"
#include <functional>

#define BULLET_LIFE 5.0f    // ���ł���܂ł̎���

#pragma region �v���g�^�C�v�錾
class Game;
class MeshComponent;
class AudioComponent;
class BulletMove;
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


class BulletMove : public MoveComponent
{
public:
	BulletMove(Actor* owner, Actor* bulletOwner, CharacterTag targetTag, std::function<void(Character*)> onHit);

	void Update(float deltaTime) override;

	// �Q�b�^�[/�Z�b�^�[
	CharacterTag GetTargetTag() { return mTargetTag; }
	void SetTargetTag(CharacterTag tag) { mTargetTag = tag; }

protected:
	// �������ɌĂ΂��R�[���o�b�N
	std::function<void(Character*)> mOnHit;

	Actor* mBulletOwner;     // �e�𔭎˂����A�N�^�[
	CharacterTag mTargetTag; // �U���Ώۂ̃^�O
};