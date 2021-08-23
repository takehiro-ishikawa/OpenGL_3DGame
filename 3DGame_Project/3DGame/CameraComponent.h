#pragma once
#include "Component.h"
#include "Math.h"

// �v���g�^�C�v�錾
class Actor;

// �J�����R���|�[�l���g�̊��N���X
class CameraComponent : public Component
{
public:
	CameraComponent(Actor* owner, int updateOrder = 200);

protected:
	void SetViewMatrix(const Matrix4& view);
};

// ��_�J�����̃R���|�[�l���g
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


#define CAMERA_MAX_ROTATE_SPEED 500.0f // �J�����̍ő��]���x
#define INPUT_CORRECTION_VALUE  20.0f  // �L�[�{�[�h�ƃR���g���[�����͂̕␳�l
#define CAMERA_TRANSITION_SPEED  8.0f  // �J������ʒu�̑J�ڑ��x

// TPS�J�����̃R���|�[�l���g
class TPSCamera : public CameraComponent
{
public:
	TPSCamera(Actor* owner);

	void ProcessInput(const struct InputState& state) override;
	void Update(float deltaTime) override;

	void SetTargetDist(float dist) { mTargetDist = dist; }
	void SetSpringConstant(float spring) { mSpringConstant = spring; }

	// �Q�b�^�[/�Z�b�^�[
	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); } // ���g�̑O���x�N�g�����擾
	Vector3 GetRight() const { return Vector3::Transform(Vector3::UnitY, mRotation); }
	Quaternion GetQuaternion() const { return mRotation; }

	float GetPitch() const { return mPitch; }                      // ���݂̃s�b�`���擾
	float GetPitchSpeed() const { return mPitchSpeed; }            // �s�b�`�̉�]/�b���x���擾
	float GetMaxPitch() const { return mMaxPitch; }                // �ő�s�b�`�p�x���擾
	Vector3 GetIdealOffsetPos() const { return mIdealOffsetPos; }  // �ʒu�̗��z�̃I�t�Z�b�g�l���擾
	float GetIdealDist() const { return mIdealCameraDist; }        // ���L�A�N�^�[�Ƃ̗��z�̋������擾
	float GetMaxYawSpeed() const { return mMaxYawSpeed; }          // ���[�̍ő��]���x���擾
	float GetMaxPitchSpeed() const { return mMaxPitchSpeed; }      // �s�b�`�̍ő��]���x���擾

	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }       // �s�b�`�̉�]/�b���x��ݒ�
	void SetMaxPitch(float pitch) { mMaxPitch = pitch; }           // �ő�s�b�`�p�x��ݒ�
	void SetIdealOffsetPos(Vector3 value) { mIdealOffsetPos = value; }       // �ʒu�̃I�t�Z�b�g�l��ݒ�
	void SetIdealDist(float dist) { mIdealCameraDist = dist; }               // ���L�A�N�^�[�Ƃ̋�����ݒ�
	void SetMaxYawSpeed(float speed) { mMaxYawSpeed = speed; }     // ���[�̍ő��]���x��ݒ�
	void SetMaxPitchSpeed(float speed) { mMaxPitchSpeed = speed; } // �s�b�`�̍ő��]���x��ݒ�

private:
	void ComputeCameraPos(float value);

	// �J�������v���C���[���痣�����������߂�
	float ComputeCameraDist(const Vector3& pos, const Vector3& dir);

	Vector3 mActualPos;    // �J�����̎��ۂ̈ʒu
	Vector3 mVelocity;     // ���ۂ̃J�����̑��x
	Quaternion mRotation;  // �J�����̉�]

	Vector3 mIdealOffsetPos;    // �ʒu�̗��z�̃I�t�Z�b�g�l
	Vector3 mActualOffsetPos;   // �ʒu�̎��ۂ̃I�t�Z�b�g�l
	float mIdealCameraDist;     // �A�N�^�[���痣�����z�̋���
	float mActualCameraDist;    // �A�N�^�[���痣�����ۂ̋���
	float mTargetDist;     // �ڕW����
	float mSpringConstant; // �΂˒萔�i�����قǍd���j

	float mPitch;          // ���݂̃s�b�`
	float mPitchSpeed;     // �s�b�`�̉�]/�b���x
	float mMaxPitch;       // �ő�s�b�`�p�x
	float mMaxPitchSpeed;  // �s�b�`�̍ő��]���x

	float mYaw;            // ���݂̃��[
	float mYawSpeed;       // ���[�̉�]/�b���x
	float mMaxYawSpeed;    // ���[�̍ő��]���x
};