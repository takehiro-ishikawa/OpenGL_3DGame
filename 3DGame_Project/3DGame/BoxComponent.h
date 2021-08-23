#pragma once
#include "Component.h"
#include "PhysWorld.h"

// �v���g�^�C�v�錾
class Actor;

// �����蔻����s���{�b�N�X�̃R���|�[�l���g
class BoxComponent : public Component
{
public:
	BoxComponent(Actor* owner, int updateOrder = 100);
	~BoxComponent();

	void OnUpdateWorldTransform() override;

	// �Q�b�^�[/�Z�b�^�[
	void SetObjectBox(const AABB& model) { mObjectBox = model; }
	const AABB& GetWorldBox() const { return mWorldBox; }
	void SetShouldRotate(bool value) { mShouldRotate = value; }

private:
	AABB mObjectBox;    // �I�u�W�F�N�g��Ԃ�AABB
	AABB mWorldBox;     // ���[���h��Ԃ�AABB
	bool mShouldRotate; // ���L�A�N�^�[�̉�]�ɍ��킹�ĉ�]�����邩
};