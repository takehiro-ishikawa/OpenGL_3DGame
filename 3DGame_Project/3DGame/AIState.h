#pragma once
#include "StateMachine.h"

// �X�e�[�g��
#define AI_IDLE "Idle" // �ҋ@
#define AI_DEAD "Dead" // ���S

class AIIdle : public State
{
public:
	AIIdle(Character* owner)
		:State(owner)
	{ }

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{
		return AI_IDLE;
	}
};

class AIDead : public State
{
public:
	AIDead(Character* owner)
		:State(owner)
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
