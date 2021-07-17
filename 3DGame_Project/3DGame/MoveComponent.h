#pragma once
#include "Component.h"
#include "Math.h"

// プロトタイプ宣言
class Actor;
class Vector2;

class MoveComponent : public Component
{
public:
	// updateOrderの値が小さいほど、先に更新される
	MoveComponent(Actor* owner, int updateOrder = 10);
	void Update(float deltaTime) override;

	Vector2 GetMoveSpeed() const { return mMoveSpeed; }
	float GetAngularSpeed() const { return mAngularSpeed; }
	void SetMoveSpeed(Vector2 speed) { mMoveSpeed = speed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	
protected:
	float mAngularSpeed; // 回転を制御する(ラジアン値)
	Vector2 mMoveSpeed;  // 前後左右移動を制御する(単位長/秒)
};
