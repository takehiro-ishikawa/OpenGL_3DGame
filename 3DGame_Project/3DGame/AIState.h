#pragma once

// プロトタイプ宣言
class AIComponent;

class AIState
{
public:
	AIState(AIComponent* owner)
		:mOwner(owner)
	{ }
	// 状態固有の動作
	virtual void Update(float deltaTime) = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	// 状態の文字列名のゲッター
	virtual const char* GetName() const = 0;

protected:
	AIComponent* mOwner;
};

class AIIdle : public AIState
{
public:
	AIIdle(AIComponent* owner)
		:AIState(owner)
	{ }

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;
};

class AIDead : public AIState
{
public:
	AIDead(AIComponent* owner)
		:AIState(owner)
	{ }

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;
};
