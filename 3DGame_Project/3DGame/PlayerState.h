#pragma once
#include "StateMachine.h"
#include "PlayerParameters.h"

// プロトタイプ宣言
class Player;

// ステート名
#define PLAYER_IDLE      "Idle"      // 待機
#define PLAYER_WALK      "Walk"      // 歩行
#define PLAYER_RUN       "Run"       // ダッシュ
#define PLAYER_SHOOT     "Shoot"     // 射撃
#define PLAYER_SHOOTWALK "ShootWalk" // 射撃&歩行

class PlayerState : public State
{
public:
	PlayerState(Character* owner);

protected: 
	Player* mPlayer;
};

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
