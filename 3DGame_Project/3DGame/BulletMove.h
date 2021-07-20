#pragma once
#include "MoveComponent.h"

#pragma region プロトタイプ宣言
class Actor;
#pragma endregion

class BulletMove : public MoveComponent
{
public:
	BulletMove(Actor* owner);

	void SetPlayer(Actor* player) { mPlayer = player; }
	void Update(float deltaTime) override;

protected:
	Actor* mPlayer;
};