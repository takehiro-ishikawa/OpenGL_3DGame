#pragma once
#include <unordered_map>
#include <string>

// �v���g�^�C�v�錾
class Character;

// �S�ẴX�e�[�g�̊��N���X
class State
{
public:
	State(Character* owner)
		:mOwner(owner)
	{ }
	// ��ԌŗL�̓���
	virtual void Input(const struct InputState& state) {};
	virtual void Update(float deltaTime) = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	// ��Ԃ̕����񖼂̃Q�b�^�[
	virtual const char* GetName() const = 0;

protected:
	Character* mOwner;
};

// �X�e�[�g�}�V���𐧌䂷����N���X
class StateMachine
{
public:
	StateMachine();

	void ChangeState(const std::string& name);

	// �A�z�z��ɐV������Ԃ�ǉ����܂�
	void RegisterState(State* state);

protected:
	// State�C���X�^���X���i�[����A�z�z��
	std::unordered_map<std::string, State*> mStateMap;

	// ���݂̏��
	State* mCurrentState;
};
