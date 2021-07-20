#pragma once
#include "StateMachine.h"

// �v���g�^�C�v�錾
class Player;

// �X�e�[�g��
#define PLAYER_IDLE "Idle" // �ҋ@
#define PLAYER_WALK "Walk" // ���s

class PlayerState : public State
{
public:
	PlayerState(Character* owner);

protected: 
	Player* mPlayer;
};

class PlayerIdle : public PlayerState
{
public:
	PlayerIdle(Character* owner)
		:PlayerState(owner)
	{ }

	void Input(const struct InputState& state) override;
	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{
		return PLAYER_IDLE;
	}
};

class PlayerWalk : public PlayerState
{
public:
	PlayerWalk(Character* owner)
		:PlayerState(owner)
	{ }

	void Input(const struct InputState& state) override;
	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{
		return PLAYER_WALK;
	}
};