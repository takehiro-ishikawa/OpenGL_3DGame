#include "MoveComponent.h"
#include "Actor.h"
#include <iostream>

MoveComponent::MoveComponent(class Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
	, mAngularSpeed(0.0f)
{

}

void MoveComponent::Update(float deltaTime)
{
	// ��]
	if (!Math::NearZero(mAngularSpeed))
	{
		// ���L�A�N�^�[�̉�]�N�H�[�^�j�I�����擾����
		Quaternion rot = mOwner->GetRotation();

		float angle = mAngularSpeed * deltaTime;

		// ����ɉ�]�����邽�߂̐V�����N�H�[�^�j�I�������
		// �i�㎲�𒆐S�ɉ�]�j
		Quaternion inc(Vector3::UnitZ, angle);

		// �V���̃N�H�[�^�j�I����A������
		rot = Quaternion::Concatenate(rot, inc);
		mOwner->SetRotation(rot);
	}

	// �O�㍶�E�ړ�
	if (!Math::NearZero(mMoveSpeed.x) || !Math::NearZero(mMoveSpeed.y))
	{
		Vector3 pos = mOwner->GetPosition();
		pos += mOwner->GetForward() * mMoveSpeed.y * deltaTime;
		pos += mOwner->GetRight() * mMoveSpeed.x * deltaTime;
		mOwner->SetPosition(pos);
	}
}