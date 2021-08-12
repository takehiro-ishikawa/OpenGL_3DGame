#pragma once
#include "Actor.h"

// �v���g�^�C�v�錾
class Game;
class MeshComponent;
class BoxComponent;

class Container : public Actor
{
public:
	Container(Game* game);

private:
	MeshComponent* mMeshComp;
	BoxComponent* mBoxComp;
};