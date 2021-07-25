#pragma once
#include "StateMachine.h"
#include "Math.h"

// プロトタイプ宣言
class Enemy;

// ステート名
#define AI_IDLE     "Idle"     // 待機
#define AI_DEAD     "Dead"     // 死亡
#define AI_VIGILANT "Vigilant" // 警戒

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
