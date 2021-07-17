#pragma once
#include "CameraComponent.h"

// �v���g�^�C�v�錾
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
	
	float mPitch; // ���݂̃s�b�`
	float mYaw;   // ���݂̃��[
};