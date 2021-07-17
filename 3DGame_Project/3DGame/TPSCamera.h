#pragma once
#include "CameraComponent.h"
#include "Math.h"

// �v���g�^�C�v�錾
class Actor;

class TPSCamera : public CameraComponent
{
public:
	TPSCamera(Actor* owner);

	void ProcessInput(const struct InputState& state) override;
	void Update(float deltaTime) override;

	void SnapToIdeal();
	void SetTargetDist(float dist) { mTargetDist = dist; }
	void SetSpringConstant(float spring) { mSpringConstant = spring; }

	float GetPitch() const { return mPitch; }           // ���݂̃s�b�`���擾
	float GetPitchSpeed() const { return mPitchSpeed; } // �s�b�`�̉�]/�b���x���擾
	float GetMaxPitch() const { return mMaxPitch; }     // �ő�s�b�`�p�x���擾
	float GetDist() const { return mDist; }             // ���L�A�N�^�[�Ƃ̋������擾

	// ���g�̑O���x�N�g�����擾
	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); }
	Vector3 GetRight() const { return Vector3::Transform(Vector3::UnitY, mRotation); }
	
	Quaternion GetQuaternion() const { return mRotation; }

	void SetPitchSpeed(float speed) { mPitchSpeed = speed; } // �s�b�`�̉�]/�b���x��ݒ�
	void SetMaxPitch(float pitch) { mMaxPitch = pitch; }     // �ő�s�b�`�p�x��ݒ�
	void SetDist(float dist) { mDist = dist; }               // ���L�A�N�^�[�Ƃ̋�����ݒ�

private:
	Vector3 ComputeCameraPos() const;

	Vector3 mActualPos; // �J�����̎��ۂ̈ʒu
	Vector3 mVelocity;  // ���ۂ̃J�����̑��x

	Quaternion mRotation; // ��]

	float mDist;          // ���L�A�N�^�[�Ƃ̋���
	float mTargetDist;    // �ڕW����
	float mSpringConstant;// �΂˒萔�i�����قǍd���j
	float mCameraZOffset;  // �J�����ʒu�̏�����̕␳�l

	float mPitch;      // ���݂̃s�b�`
	float mPitchSpeed; // �s�b�`�̉�]/�b���x
	float mMaxPitch;   // �ő�s�b�`�p�x

	float mYaw;        // ���݂̃��[
	float mYawSpeed;   // ���[�̉�]/�b���x
};
