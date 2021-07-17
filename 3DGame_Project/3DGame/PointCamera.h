#pragma once
#include "CameraComponent.h"

// プロトタイプ宣言
class Actor;

class PointCamera : public CameraComponent
{
public:
	PointCamera(Actor* owner);

	void Update(float deltaTime) override;

	float GetPitch() const { return mPitch; }
	float GetYaw() const { return mYaw; }

	void SetPitch(float speed) { mPitch = speed; }
	void SetYaw(float speed) { mYaw = speed; }

private:
	
	float mPitch; // 現在のピッチ
	float mYaw;   // 現在のヨー
};