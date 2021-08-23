#pragma once
#include <unordered_map>
#include <string>
#include "Math.h"

// プロトタイプ宣言
class Character;
class Player;
class Enemy;

// 全てのステートの基底クラス
class State
{
public:
	State(Character* owner)
		:mOwner(owner)
	{ }
	// 状態固有の動作
	virtual void Input(const struct InputState& state) {};
	virtual void Update(float deltaTime) = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	// 状態の文字列名のゲッター
	virtual const char* GetName() const = 0;

protected:
	Character* mOwner;
};

// ステートマシンを制御する基底クラス
class StateMachine
{
public:
	StateMachine();

	void ChangeState(const std::string& name);

	// 連想配列に新しい状態を追加します
	void RegisterState(State* state);

protected:
	// Stateインスタンスを格納する連想配列
	std::unordered_map<std::string, State*> mStateMap;

	// 現在の状態
	State* mCurrentState;
};

#pragma region プレイヤーのステート

// ステート名
#define PLAYER_IDLE      "Idle"      // 待機状態
#define PLAYER_WALK      "Walk"      // 歩行状態
#define PLAYER_RUN       "Run"       // ダッシュ状態
#define PLAYER_SHOOT     "Shoot"     // 射撃状態
#define PLAYER_SHOOTWALK "ShootWalk" // 射撃&歩行状態
#define PLAYER_ATTACK    "Attack"    // 近接攻撃状態
#define PLAYER_DEAD      "Dead"      // 死亡状態

// プレイヤーステートの基底クラス
class PlayerState : public State
{
public:
	PlayerState(Character* owner);

protected:
	Player* mPlayer;
};

// "待機"状態
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

// "歩行"状態
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

// "ダッシュ"状態
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

// "射撃"状態
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

// "射撃&歩行"状態
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

// "近接攻撃"状態
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

// "死亡"状態
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


#pragma region 敵のステート

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
