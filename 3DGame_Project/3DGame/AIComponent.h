#pragma once
#include "Component.h"
#include <unordered_map>
#include <string>

class AIComponent : public Component
{
public:
	AIComponent(class Actor* owner);

	void Update(float deltaTime) override;
	void ChangeState(const std::string& name);

	// 連想配列に新しい状態を追加します
	void RegisterState(class AIState* state);

private:
	// AIStateインスタンスを格納する連想配列
	std::unordered_map<std::string, class AIState*> mStateMap;

	// 現在の状態
	class AIState* mCurrentState;
};
