#pragma once
#include "StateMachine.h"
#include "PlayerParameters.h"

// �v���g�^�C�v�錾
class Player;

// �X�e�[�g��
#define PLAYER_IDLE      "Idle"      // �ҋ@���
#define PLAYER_WALK      "Walk"      // ���s���
#define PLAYER_RUN       "Run"       // �_�b�V�����
#define PLAYER_SHOOT     "Shoot"     // �ˌ����
#define PLAYER_SHOOTWALK "ShootWalk" // �ˌ�&���s���
#define PLAYER_ATTACK    "Attack"    // �ߐڍU�����
#define PLAYER_DEAD      "Dead"      // ���S���

// �v���C���[�X�e�[�g�̊��N���X
class PlayerState : public State
{
public:
	PlayerState(Character* owner);

protected: 
	Player* mPlayer;
};

// "�ҋ@"���
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

// "���s"���
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

// "�_�b�V��"���
class PlayerRun : public PlayerState
{
public:
	PlayerRun(Character* owner)
		:PlayerState(owner)
	{ }

	void Input(const struct InputState& state) override;
	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{
		return PLAYER_RUN;
	}
};

// "�ˌ�"���
class PlayerShoot : public PlayerState
{
public:
	PlayerShoot(Character* owner)
		:PlayerState(owner)
	{ }

	void Input(const struct InputState& state) override;
	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{
		return PLAYER_SHOOT;
	}
};

// "�ˌ�&���s"���
class PlayerShootWalk : public PlayerState
{
public:
	PlayerShootWalk(Character* owner)
		:PlayerState(owner)
	{ }

	void Input(const struct InputState& state) override;
	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{
		return PLAYER_SHOOTWALK;
	}
};

// "�ߐڍU��"���
class PlayerAttack : public PlayerState
{
public:
    PlayerAttack(Character* owner)
		:PlayerState(owner)
		, mAttackTime(0)
		, mEndTime(0)
	{ }

	void Input(const struct InputState& state) override;
	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{
		return PLAYER_ATTACK;
	}

private:
	float mAttackTime;
	float mEndTime;
};

// "���S"���
class PlayerDead : public PlayerState
{
public:
	PlayerDead(Character* owner)
		:PlayerState(owner)
		,mTimeCount(0)
	{ }

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{
		return PLAYER_DEAD;
	}

private:
	float mTimeCount;
};