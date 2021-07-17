#pragma once
#include <cstdint>

// �v���g�^�C�v�錾
class Actor;

class Component
{
public:
	// �R���X�g���N�^
	//�i�X�V�������Ⴂ�قǁA�R���|�[�l���g�̍X�V�������Ȃ�j
	Component(Actor* owner, int updateOrder = 100);

	// �f�X�g���N�^
	virtual ~Component();

	// ���̃R���|�[�l���g���f���^�^�C���ōX�V����
	virtual void Update(float deltaTime);

	// ���̃R���|�[�l���g�̃v���Z�X����
	virtual void ProcessInput(const struct InputState& state) {}

	// ���[���h���W���ω��������ɌĂяo�����
	virtual void OnUpdateWorldTransform() { }

	Actor* GetOwner() { return mOwner; }
	int GetUpdateOrder() const { return mUpdateOrder; }

protected:
	// ���L�A�N�^�[
	Actor* mOwner;
	// �R���|�[�l���g�̏������X�V����
	int mUpdateOrder;
};

