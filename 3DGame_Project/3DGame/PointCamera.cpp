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
	// 親のUpdateを呼び出す
	CameraComponent::Update(deltaTime);

	// カメラの位置は所有アクターの位置
	Vector3 cameraPos = mOwner->GetPosition();

	// 回転を表すクォータニオンを作成する
	Quaternion q_pitch(mOwner->GetRight(), mPitch);
	Quaternion q_yaw(mOwner->GetUpward(), mYaw);
	Quaternion q = Quaternion::Concatenate(q_pitch, q_yaw);

	// ピッチクォータニオンで所有アクターの前方ベクトルを回転させる
	Vector3 viewForward = Vector3::Transform(mOwner->GetForward(), q);

	// ターゲットの位置は所有アクターの前方100単位
	Vector3 target = cameraPos + viewForward * 100.0f;

	// 上方向ベクトルもピッチのクォータニオンで回転する
	Vector3 up = Vector3::Transform(Vector3::UnitZ, q);

	// 注視行列を作ってビューに設定する
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view);
}