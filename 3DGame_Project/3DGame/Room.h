#pragma once
#include "Actor.h"

// �v���g�^�C�v�錾
class Game;

// �������쐬����N���X
class Room : public Actor
{
public:
	Room(Game* game, Vector3 position, Vector3 size);
	
private:
	
	// �����̃T�C�Y
	Vector3 mSize;
};