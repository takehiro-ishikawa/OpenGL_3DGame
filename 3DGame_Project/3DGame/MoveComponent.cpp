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
	// 回転
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
		pos += mOwner->GetForward() * mMoveSpeed.y * deltaTime;
		pos += mOwner->GetRight() * mMoveSpeed.x * deltaTime;
		mOwner->SetPosition(pos);
	}
}