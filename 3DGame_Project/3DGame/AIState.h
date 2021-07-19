#pragma once

// �v���g�^�C�v�錾
class AIComponent;

class AIState
{
public:
	AIState(AIComponent* owner)
		:mOwner(owner)
	{ }
	// ��ԌŗL�̓���
	virtual void Update(float deltaTime) = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	// ��Ԃ̕����񖼂̃Q�b�^�[
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
