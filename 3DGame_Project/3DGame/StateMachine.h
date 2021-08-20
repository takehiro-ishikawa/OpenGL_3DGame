#pragma once
#include <unordered_map>
#include <string>

// プロトタイプ宣言
class Character;

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
