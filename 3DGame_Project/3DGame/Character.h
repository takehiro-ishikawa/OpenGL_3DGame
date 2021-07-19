#pragma once
#include "Actor.h"

#pragma region �v���g�^�C�v�錾
class Game;
class BoxComponent;
class AudioComponent;
#pragma endregion

class Character : public Actor
{
public:
	Character(Game* game);

protected:
	// �u�ŏ��̏d�Ȃ�v�ŏՓ˂��e�X�g����
	void FixCollisions();

	BoxComponent* mBoxComp;
	AudioComponent* mAudioComp;
};