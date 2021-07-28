#pragma once
#include "Actor.h"

#define PLANE_SIZE 500.0f // ���̃T�C�Y

// �v���g�^�C�v�錾
class Game;
class BoxComponent;

class PlaneActor : public Actor
{
public:
	PlaneActor(Game* game, float scale = 1.0f);
	~PlaneActor();
	BoxComponent* GetBox() { return mBox; }

private:
	BoxComponent* mBox;
};