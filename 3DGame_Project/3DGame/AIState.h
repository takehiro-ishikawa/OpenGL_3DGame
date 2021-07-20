#pragma once
#include "StateMachine.h"

// ステート名
#define AI_IDLE "Idle" // 待機
#define AI_DEAD "Dead" // 死亡

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
