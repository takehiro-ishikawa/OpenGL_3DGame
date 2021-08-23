#pragma once
#include <unordered_map>
#include <string>
#include "Math.h"

// �v���g�^�C�v�錾
class Character;
class Player;
class Enemy;

// �S�ẴX�e�[�g�̊��N���X
class State
{
public:
	State(Character* owner)
		:mOwner(owner)
	{ }
	// ��ԌŗL�̓���
	virtual void Input(const struct InputState& state) {};
	virtual void Update(float deltaTime) = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	// ��Ԃ̕����񖼂̃Q�b�^�[
	virtual const char* GetName() const = 0;

protected:
	Character* mOwner;
};

// �X�e�[�g�}�V���𐧌䂷����N���X
class StateMachine
{
public:
	StateMachine();

	void ChangeState(const std::string& name);

	// �A�z�z��ɐV������Ԃ�ǉ����܂�
	void RegisterState(State* state);

protected:
	// State�C���X�^���X���i�[����A�z�z��
	std::unordered_map<std::string, State*> mStateMap;

	// ���݂̏��
	State* mCurrentState;
};

#pragma region �v���C���[�̃X�e�[�g

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
		, mTimeCount(0)
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

#pragma endregion


#pragma region �G�̃X�e�[�g

// �X�e�[�g��
#define AI_IDLE     "Idle"     // �ҋ@���
#define AI_DEAD     "Dead"     // ���S���
#define AI_VIGILANT "Vigilant" // �x�����
#define AI_ATTACK   "Attack"   // �U�����

// �GAI�X�e�[�g�N���X�̊��N���X
class AIState : public State
{
public:
	AIState(Character* owner);

protected:
	Enemy* mEnemy;
};

// "�ҋ@"��Ԃ̓GAI�̃X�e�[�g
class AIIdle : public AIState
{
public:
	AIIdle(Character* owner)
		:AIState(owner)
	{ }

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{
		return AI_IDLE;
	}
};

// "���S"��Ԃ̓GAI�̃X�e�[�g
class AIDead : public AIState
{
public:
	AIDead(Character* owner)
		:AIState(owner)
		, mLifeSpan(0)
	{ }

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{
		return AI_DEAD;
	}

private:
	float mLifeSpan;
};

// "�x��"��Ԃ̓GAI�̃X�e�[�g
class AIVigilant : public AIState
{
public:
	AIVigilant(Character* owner)
		:AIState(owner)
	{ }

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{
		return AI_VIGILANT;
	}

private:

	// ��]����莞�Ԓ�~����
	void Rest();

	bool mIsRotate;   // ��]�����H
	float mRestTime;  // �Î~���Ă��鎞��
	Vector3 mNextDir; // ���ɉ�]���~���������
};

// "�U��"��Ԃ̓GAI�̃X�e�[�g
class AIAttack : public AIState
{
public:
	AIAttack(Character* owner)
		:AIState(owner)
		, mIsChase(false)
	{ }

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{
		return AI_ATTACK;
	}

private:
	float mAttackSpan;
	bool mIsChase;
};

#pragma endregion
