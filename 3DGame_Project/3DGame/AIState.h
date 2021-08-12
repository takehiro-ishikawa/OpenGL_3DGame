#pragma once
#include "StateMachine.h"
#include "Math.h"

// �v���g�^�C�v�錾
class Enemy;

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
		,mLifeSpan(0)
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
		,mIsChase(false)
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