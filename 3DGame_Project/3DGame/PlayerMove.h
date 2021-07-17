#pragma once
#include "MoveComponent.h"

// プロトタイプ宣言
class Actor;

class PlayerMove : public MoveComponent
{
public:
	PlayerMove(Actor* owner, int updateOrder = 10);
	void Update(float deltaTime) override;

	Vector3 GetForward() const { return mForward; }
	Vector3 GetRight() const { return mRight; }
	float GetJumpSpeed() const { return mJumpSpeed; }
	void SetForward(Vector3 value) { mForward = value; }
	void SetRight(Vector3 value) { mRight = value; }
	void SetJumpSpeed(float speed) { mJumpSpeed = speed; }
	void SetIsFall(bool value) { mIsFall = value; }

private:
	Vector3 mForward; // 前方ベクトル
	Vector3 mRight;   // 後方ベクトル

	float mJumpSpeed;
	bool mIsFall;
};