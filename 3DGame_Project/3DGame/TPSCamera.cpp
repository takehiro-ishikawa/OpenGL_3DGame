#include "TPSCamera.h"
#include "PlayerParameters.h"
#include "Actor.h"
#include "InputSystem.h"
#include <iostream>

TPSCamera::TPSCamera(Actor* owner)
	:CameraComponent(owner)
	, mRotation(Quaternion::Identity)
	, mCameraDist(CAMERA_NORMAL_DIST)
	, mOffsetPos(CAMERA_NORMAL_OFFSETPOS)
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

	// マウスでの視点操作
	InputDevice device;
	Vector2 rightAxis = state.GetMappedAxis(INPUT_RIGHT_AXIS, device);
	if (device == InputDevice::EKeyBoard || device == InputDevice::EController) rightAxis *= INPUT_CORRECTION_VALUE;

	// ヨーを設定
	float yawSpeed = 0.0f;
	if (rightAxis.x != 0)
	{
		yawSpeed = static_cast<float>(rightAxis.x) / CAMERA_MAX_ROTATE_SPEED;
		yawSpeed *= mMaxYawSpeed;
	}
	mYawSpeed = yawSpeed;

	// ピッチを設定
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

	// カメラの位置は所有アクターの位置からmCameraZOffsetだけ上
	Vector3 cameraPos = mOwner->GetPosition();
	cameraPos += Vector3::UnitZ * mOffsetPos.z;
	cameraPos += GetRight()     * mOffsetPos.x;
	cameraPos += GetForward()   * mOffsetPos.y;

	// ---------- ヨー回転処理 ---------- //
	// ヨーの角速度に基づいてヨーを更新する
	mYaw = mYawSpeed * deltaTime;

	// ヨー回転を表すクォータニオンを作成する
	Quaternion q(Vector3::UnitZ, mYaw);

	// ヨークォータニオンで"自身"を回転させる
	mRotation = Quaternion::Concatenate(mRotation, q);

	// ---------- ピッチ回転処理 ---------- //
	// ピッチの角速度に基づいてピッチを更新する
	mPitch += mPitchSpeed * deltaTime;
	// ピッチを[-max、+ max]にクランプする
	mPitch = Math::Clamp(mPitch, -mMaxPitch, mMaxPitch);

	// ピッチ回転を表すクォータニオンを作成する
	// （自身の右向きベクトルを軸とする回転
	Vector3 cameraRight = Vector3::Transform(Vector3::UnitY, mRotation);
	q = Quaternion(cameraRight, mPitch);

	// ピッチクォータニオンで"自身の前方ベクトル"を回転させる
	Vector3 cameraForward = Vector3::Transform(Vector3::UnitX, mRotation);
	Vector3 viewForward = Vector3::Transform(cameraForward, q);

	// 所有アクターからmDistだけ離す
	cameraPos -= viewForward * mCameraDist;

	// ターゲットの位置は所有アクターの前方100単位
	Vector3 target = cameraPos + viewForward * 100.0f;

	// 上方向ベクトルもピッチのクォータニオンで回転する
	Vector3 up = Vector3::Transform(Vector3::UnitZ, mRotation);


	// 注視行列を作ってビューに設定する
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view);
}

void TPSCamera::SnapToIdeal()
{
	// 実際の位置を理想の位置と同じに設定
	mActualPos = ComputeCameraPos();

	// 速度はゼロ
	mVelocity = Vector3::Zero;

	// ターゲットを計算して表示する
	Vector3 target = mOwner->GetPosition() + mOwner->GetForward() * mTargetDist;

	// ここでは実際の位置を使用しますが、理想的ではない
	Matrix4 view = Matrix4::CreateLookAt(mActualPos, target, Vector3::UnitZ);

	SetViewMatrix(view);
}

Vector3 TPSCamera::ComputeCameraPos() const
{
	// カメラの位置を所有アクターの上後方にセット
	Vector3 cameraPos = mOwner->GetPosition();
	//cameraPos -= mOwner->GetForward() * mHorzDist;
	//cameraPos += Vector3::UnitZ * mVertDist;
	return cameraPos;
}