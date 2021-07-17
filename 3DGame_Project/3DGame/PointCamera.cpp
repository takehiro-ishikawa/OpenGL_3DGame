#include "PointCamera.h"
#include "Actor.h"

PointCamera::PointCamera(Actor* owner)
	:CameraComponent(owner)
	, mPitch(0.0f)
	, mYaw(0.0f)
{
}

void PointCamera::Update(float deltaTime)
{
	// �e��Update���Ăяo��
	CameraComponent::Update(deltaTime);

	// �J�����̈ʒu�͏��L�A�N�^�[�̈ʒu
	Vector3 cameraPos = mOwner->GetPosition();

	// ��]��\���N�H�[�^�j�I�����쐬����
	Quaternion q_pitch(mOwner->GetRight(), mPitch);
	Quaternion q_yaw(mOwner->GetUpward(), mYaw);
	Quaternion q = Quaternion::Concatenate(q_pitch, q_yaw);

	// �s�b�`�N�H�[�^�j�I���ŏ��L�A�N�^�[�̑O���x�N�g������]������
	Vector3 viewForward = Vector3::Transform(mOwner->GetForward(), q);

	// �^�[�Q�b�g�̈ʒu�͏��L�A�N�^�[�̑O��100�P��
	Vector3 target = cameraPos + viewForward * 100.0f;

	// ������x�N�g�����s�b�`�̃N�H�[�^�j�I���ŉ�]����
	Vector3 up = Vector3::Transform(Vector3::UnitZ, q);

	// �����s�������ăr���[�ɐݒ肷��
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view);
}