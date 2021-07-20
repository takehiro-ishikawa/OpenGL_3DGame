#pragma once
#include "MoveComponent.h"

#pragma region �v���g�^�C�v�錾
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