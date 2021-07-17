#pragma once
#include "Actor.h"

// プロトタイプ宣言
class Game;
class BoxComponent;

class PlaneActor : public Actor
{
public:
	PlaneActor(Game* game);
	~PlaneActor();
	BoxComponent* GetBox() { return mBox; }

private:
	BoxComponent* mBox;
};