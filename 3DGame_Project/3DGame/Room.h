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
	void GeneratePlanes(float posZ);
	void GenerateWall();

	Vector3 mSize;        // �����̃T�C�Y
	Vector3 mMinPosition; // �������̍ŏ����W
	Vector3 mMaxPosition; // �������̍ő���W
};