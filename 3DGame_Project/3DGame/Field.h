#pragma once
#include "Actor.h"

// �v���g�^�C�v�錾
class Game;
class MeshComponent;
class BoxComponent;

#define PLANE_SIZE 500.0f // ���̃T�C�Y

// ���I�u�W�F�N�g
class PlaneActor : public Actor
{
public:
	PlaneActor(Game* game, float scale = 1.0f);
	~PlaneActor();
	BoxComponent* GetBox() { return mBox; }

private:
	BoxComponent* mBox;
};

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

// �t�B�[���h��ɒu���R���e�i
class Container : public Actor
{
public:
	Container(Game* game);

private:
	MeshComponent* mMeshComp;
	BoxComponent* mBoxComp;
};