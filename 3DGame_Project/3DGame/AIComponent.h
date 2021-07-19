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

	// �A�z�z��ɐV������Ԃ�ǉ����܂�
	void RegisterState(class AIState* state);

private:
	// AIState�C���X�^���X���i�[����A�z�z��
	std::unordered_map<std::string, class AIState*> mStateMap;

	// ���݂̏��
	class AIState* mCurrentState;
};
