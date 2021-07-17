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
		// 所有アクターの回転クォータニオンを取得する
		Quaternion rot = mOwner->GetRotation();

		float angle = mAngularSpeed * deltaTime;

		// さらに回転させるための新しいクォータニオンを作る
		// （上軸を中心に回転）
		Quaternion inc(Vector3::UnitZ, angle);

		// 新旧のクォータニオンを連結する
		rot = Quaternion::Concatenate(rot, inc);
		mOwner->SetRotation(rot);
	}

	// 前後左右移動
	if (!Math::NearZero(mMoveSpeed.x) || !Math::NearZero(mMoveSpeed.y))
	{
		Vector3 pos = mOwner->GetPosition();
		pos += mForward * mMoveSpeed.y * deltaTime;
		pos += mRight * mMoveSpeed.x * deltaTime;
		mOwner->SetPosition(pos);
	}

	// 上下移動（ジャンプ、落下）
	if (mIsFall == true)
	{
		Vector3 pos = mOwner->GetPosition();
		pos += mOwner->GetUpward() * mJumpSpeed * deltaTime;
		mOwner->SetPosition(pos);

		mJumpSpeed -= 20;
	}
}