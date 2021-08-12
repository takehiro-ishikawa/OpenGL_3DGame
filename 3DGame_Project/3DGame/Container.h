#pragma once
#include "Actor.h"

// プロトタイプ宣言
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