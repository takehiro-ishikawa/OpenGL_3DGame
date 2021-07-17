#include "PlayerMove.h"
#include "Actor.h"

PlayerMove::PlayerMove(class Actor* owner, int updateOrder)
	:MoveComponent(owner, updateOrder)
{
}

void PlayerMove::Update(float deltaTime)
{
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
		pos += mForward * mMoveSpeed.y * deltaTime;
		pos += mRight * mMoveSpeed.x * deltaTime;
		mOwner->SetPosition(pos);
	}

	// �㉺�ړ��i�W�����v�A�����j
	if (mIsFall == true)
	{
		Vector3 pos = mOwner->GetPosition();
		pos += mOwner->GetUpward() * mJumpSpeed * deltaTime;
		mOwner->SetPosition(pos);

		mJumpSpeed -= 20;
	}
}