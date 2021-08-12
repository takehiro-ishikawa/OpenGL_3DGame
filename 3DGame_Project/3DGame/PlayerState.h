#pragma once
#include "StateMachine.h"
#include "PlayerParameters.h"

// プロトタイプ宣言
class Player;

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