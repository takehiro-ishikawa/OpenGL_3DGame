#include "TPSCamera.h"
#include "Actor.h"
#include "InputSystem.h"
#include <iostream>

TPSCamera::TPSCamera(Actor* owner)
	:CameraComponent(owner)
	, mRotation(Quaternion::Identity)
	, mDist(550.0f)
	, mCameraZOffset(350.0f)
	, mTargetDist(100.0f)
	, mSpringConstant(1000.0f)
	, mMaxPitch(Math::Pi / 5.0f)
{
}

void TPSCamera::ProcessInput(const struct InputState& state)
{
	float angularSpeed = 0.0f;

	// �}�E�X�ł̎��_����
	InputDevice device;
	Vector2 rightAxis = state.GetMappedAxis(INPUT_RIGHT_AXIS, device);
	if (device == InputDevice::EKeyBoard || device == InputDevice::EController) rightAxis *= 20;

	// 1�t���[���ł̍ő�ړ��ʁi�}�E�X�̓����͒ʏ�-500����+500�̊ԂŐݒ�j
	const int maxMouseSpeed = 500;
    // �ő��]���x
	const float maxYawSpeed = Math::Pi * 25;
	const float maxPitchSpeed = Math::Pi * 8;

	// ���[��ݒ�
	float yawSpeed = 0.0f;
	if (rightAxis.x != 0)
	{
		yawSpeed = static_cast<float>(rightAxis.x) / maxMouseSpeed;
		yawSpeed *= maxYawSpeed;
	}
	mYawSpeed = yawSpeed;

	// �s�b�`��ݒ�
	float pitchSpeed = 0.0f;
	if (rightAxis.y != 0)
	{
		pitchSpeed = static_cast<float>(rightAxis.y) / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	mPitchSpeed = pitchSpeed;
}

void TPSCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);

	// �J�����̈ʒu�͏��L�A�N�^�[�̈ʒu����mCameraZOffset������
	Vector3 cameraPos = mOwner->GetPosition();
	cameraPos += Vector3::UnitZ * mCameraZOffset;

	// ---------- ���[��]���� ---------- //
	// ���[�̊p���x�Ɋ�Â��ă��[���X�V����
	mYaw = mYawSpeed * deltaTime;

	// ���[��]��\���N�H�[�^�j�I�����쐬����
	Quaternion q(Vector3::UnitZ, mYaw);

	// ���[�N�H�[�^�j�I����"���g"����]������
	mRotation = Quaternion::Concatenate(mRotation, q);

	// ---------- �s�b�`��]���� ---------- //
	// �s�b�`�̊p���x�Ɋ�Â��ăs�b�`���X�V����
	mPitch += mPitchSpeed * deltaTime;
	// �s�b�`��[-max�A+ max]�ɃN�����v����
	mPitch = Math::Clamp(mPitch, -mMaxPitch, mMaxPitch);

	// �s�b�`��]��\���N�H�[�^�j�I�����쐬����
	// �i���g�̉E�����x�N�g�������Ƃ����]
	Vector3 cameraRight = Vector3::Transform(Vector3::UnitY, mRotation);
	q = Quaternion(cameraRight, mPitch);

	// �s�b�`�N�H�[�^�j�I����"���g�̑O���x�N�g��"����]������
	Vector3 cameraForward = Vector3::Transform(Vector3::UnitX, mRotation);
	Vector3 viewForward = Vector3::Transform(cameraForward, q);

	// ���L�A�N�^�[����mDist��������
	cameraPos -= viewForward * mDist;

	// �^�[�Q�b�g�̈ʒu�͏��L�A�N�^�[�̑O��100�P��
	Vector3 target = cameraPos + viewForward * 100.0f;

	// ������x�N�g�����s�b�`�̃N�H�[�^�j�I���ŉ�]����
	Vector3 up = Vector3::Transform(Vector3::UnitZ, mRotation);


	// �����s�������ăr���[�ɐݒ肷��
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view);
}

void TPSCamera::SnapToIdeal()
{
	// ���ۂ̈ʒu�𗝑z�̈ʒu�Ɠ����ɐݒ�
	mActualPos = ComputeCameraPos();

	// ���x�̓[��
	mVelocity = Vector3::Zero;

	// �^�[�Q�b�g���v�Z���ĕ\������
	Vector3 target = mOwner->GetPosition() + mOwner->GetForward() * mTargetDist;

	// �����ł͎��ۂ̈ʒu���g�p���܂����A���z�I�ł͂Ȃ�
	Matrix4 view = Matrix4::CreateLookAt(mActualPos, target, Vector3::UnitZ);

	SetViewMatrix(view);
}

Vector3 TPSCamera::ComputeCameraPos() const
{
	// �J�����̈ʒu�����L�A�N�^�[�̏����ɃZ�b�g
	Vector3 cameraPos = mOwner->GetPosition();
	//cameraPos -= mOwner->GetForward() * mHorzDist;
	//cameraPos += Vector3::UnitZ * mVertDist;
	return cameraPos;
}