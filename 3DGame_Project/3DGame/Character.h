#pragma once
#include "Actor.h"

#pragma region �v���g�^�C�v�錾
class Game;
class SkeletalMeshComponent;
class BoxComponent;
class AudioComponent;
#pragma endregion

class Character : public Actor
{
public:
	Character(Game* game);

	virtual void Damage() {};

	// �Q�b�^�[
	SkeletalMeshComponent* GetMeshComp() { return mMeshComp; }
	BoxComponent* GetBoxComp() { return mBoxComp; }
	AudioComponent* GetAudioComp() { return mAudioComp; }

protected:

	// ���n����
	bool CheckGround();

	// �u�ŏ��̏d�Ȃ�v�ŏՓ˂��e�X�g����
	void FixCollisions();

	SkeletalMeshComponent* mMeshComp;
	BoxComponent* mBoxComp;
	AudioComponent* mAudioComp;
};