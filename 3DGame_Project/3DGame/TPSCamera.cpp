#include "CameraComponent.h"
#include "PlayerParameters.h"
#include "Actor.h"
#include "Game.h"
#include "InputSystem.h"
#include "PhysWorld.h"
#include <iostream>

TPSCamera::TPSCamera(Actor* owner)
	:CameraComponent(owner)
	, mRotation(Quaternion::Identity)
	, mIdealCameraDist(CAMERA_NORMAL_DIST)
	, mActualCameraDist(CAMERA_NORMAL_DIST)
	, mIdealOffsetPos(CAMERA_NORMAL_OFFSETPOS)
	, mActualOffsetPos(CAMERA_NORMAL_OFFSETPOS)
	, mTargetDist(100.0f)
	, mSpringConstant(1000.0f)
	, mMaxPitch(Math::Pi / 5.0f)
	, mMaxYawSpeed(CAMERA_NORMAL_YAWSPEED)
	, mMaxPitchSpeed(CAMERA_NORMAL_PITCHSPEED)
{
}

void TPSCamera::ProcessInput(const struct InputState& state)
{
	float angularSpeed = 0.0f;

	// �}�E�X�ł̎��_����
	InputDevice device;
	Vector2 rightAxis = state.GetMappedAxis(INPUT_RIGHT_AXIS, device);
	if (device == InputDevice::EKeyBoard || device == InputDevice::EController) rightAxis *= INPUT_CORRECTION_VALUE;

	// ���[��ݒ�
	float yawSpeed = 0.0f;
	if (rightAxis.x != 0)
	{
		yawSpeed = static_cast<float>(rightAxis.x) / CAMERA_MAX_ROTATE_SPEED;
		yawSpeed *= mMaxYawSpeed;
	}
	mYawSpeed = yawSpeed;

	// �s�b�`��ݒ�
	float pitchSpeed = 0.0f;
	if (rightAxis.y != 0)
	{
		pitchSpeed = static_cast<float>(rightAxis.y) / CAMERA_MAX_ROTATE_SPEED;
		pitchSpeed *= mMaxPitchSpeed;
	}
	mPitchSpeed = pitchSpeed;
}

void TPSCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);
	ComputeCameraPos(deltaTime * CAMERA_TRANSITION_SPEED);

	// �J�����̈ʒu�͏��L�A�N�^�[�̈ʒu����mCameraZOffset������
	Vector3 cameraPos = mOwner->GetPosition();
	cameraPos += Vector3::UnitZ * mActualOffsetPos.z;
	cameraPos += GetRight()     * mActualOffsetPos.x;
	cameraPos += GetForward()   * mActualOffsetPos.y;

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

	// ���L�A�N�^�[����J�����𗣂�
	float cameraDist = ComputeCameraDist(cameraPos, viewForward);
	cameraPos -= viewForward * cameraDist;

	// �^�[�Q�b�g�̈ʒu�͏��L�A�N�^�[�̑O��100�P��
	Vector3 target = cameraPos + viewForward * 100.0f;

	// ������x�N�g�����s�b�`�̃N�H�[�^�j�I���ŉ�]����
	Vector3 up = Vector3::Transform(Vector3::UnitZ, mRotation);

	// �����s�������ăr���[�ɐݒ肷��
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view);
}

void TPSCamera::ComputeCameraPos(float value)
{
	mActualCameraDist = Math::Lerp(mActualCameraDist, mIdealCameraDist, value);
	mActualOffsetPos = Vector3::Lerp(mActualOffsetPos, mIdealOffsetPos, value);
}

float TPSCamera::ComputeCameraDist(const Vector3& pos, const Vector3& dir)
{
	Vector3 end = pos - dir * mActualCameraDist;
	LineSegment l(pos, end);

	// �{�����������̓r���ŉ����ɐڐG���邩���m�F����
	PhysWorld* phys = mOwner->GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;
	// �ڐG����
	if (phys->SegmentCast(l, info, mOwner))
	{
		// �Ԃ������ʒu�܂ł̋�����Ԃ�
		Vector3 cameraDist = info.mPoint - pos;
		return cameraDist.Length();
	}

	// �ڐG���Ȃ������Ȃ�{������������Ԃ�
	return mActualCameraDist;
}