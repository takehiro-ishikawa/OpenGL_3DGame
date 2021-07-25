#pragma once
#include "StateMachine.h"
#include "Math.h"

// �v���g�^�C�v�錾
class Enemy;

// �X�e�[�g��
#define AI_IDLE     "Idle"     // �ҋ@
#define AI_DEAD     "Dead"     // ���S
#define AI_VIGILANT "Vigilant" // �x��

class AIState : public State
{
public:
	AIState(Character* owner);

protected:
	Enemy* mEnemy;
};

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
	void Rest();
	bool mIsRotate;
	float mRestTime;
	Vector3 mNextDir;
};
