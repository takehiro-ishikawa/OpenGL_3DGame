#pragma once
#include "Actor.h"

#define CHECKGROUND_RENGE  2.0f // ���n���莞�̐����̒���
#define CHECKGROUND_OFFSET 1.0f // ���n���莞�̐����̊J�n�ʒu��Z�����̃I�t�Z�b�g�l

#pragma region �v���g�^�C�v�錾
class Game;
class SkeletalMeshComponent;
class BoxComponent;
class AudioComponent;
#pragma endregion

enum class CharacterTag
{
	EPlayer, // �v���C���[
	EEnemy,  // �G�L����
	ENone    // ���̑�
};

class Character : public Actor
{
public:
	Character(Game* game);

	virtual void Damage(float damage) {};
	virtual void Dead() {};

	// �Q�b�^�[
	CharacterTag GetCharacterTag() { return mTag; }
	SkeletalMeshComponent* GetMeshComp() { return mMeshComp; }
	BoxComponent* GetBoxComp() { return mBoxComp; }
	AudioComponent* GetAudioComp() { return mAudioComp; }
	float GetHealth() { return mHealth; }
	void SetCharacterTag(CharacterTag tag) { mTag = tag; }
	void SetHealth(float value) { mHealth = value; }

protected:

	// ���n����
	bool CheckGround();

	// �u�ŏ��̏d�Ȃ�v�ŏՓ˂��e�X�g����
	void FixCollisions();

	SkeletalMeshComponent* mMeshComp;
	BoxComponent* mBoxComp;
	AudioComponent* mAudioComp;

	// ���g�̃L�����N�^�[�^�O
	CharacterTag mTag;

	// �̗�
	float mHealth;
};