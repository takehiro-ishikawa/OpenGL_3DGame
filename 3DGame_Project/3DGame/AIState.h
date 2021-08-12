#pragma once
#include "StateMachine.h"
#include "Math.h"

// プロトタイプ宣言
class Enemy;

// ステート名
#define AI_IDLE     "Idle"     // 待機状態
#define AI_DEAD     "Dead"     // 死亡状態
#define AI_VIGILANT "Vigilant" // 警戒状態
#define AI_ATTACK   "Attack"   // 攻撃状態

// 敵AIステートクラスの基底クラス
class AIState : public State
{
public:
	AIState(Character* owner);

protected:
	Enemy* mEnemy;
};

// "待機"状態の敵AIのステート
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

// "死亡"状態の敵AIのステート
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

// "警戒"状態の敵AIのステート
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

	// 回転を一定時間停止する
	void Rest();

	bool mIsRotate;   // 回転中か？
	float mRestTime;  // 静止している時間
	Vector3 mNextDir; // 次に回転を停止させる方向
};

// "攻撃"状態の敵AIのステート
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