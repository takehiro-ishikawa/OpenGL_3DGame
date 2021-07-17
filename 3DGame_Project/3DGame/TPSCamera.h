#pragma once
#include "CameraComponent.h"
#include "Math.h"

// プロトタイプ宣言
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

	float GetPitch() const { return mPitch; }           // 現在のピッチを取得
	float GetPitchSpeed() const { return mPitchSpeed; } // ピッチの回転/秒速度を取得
	float GetMaxPitch() const { return mMaxPitch; }     // 最大ピッチ角度を取得
	float GetDist() const { return mDist; }             // 所有アクターとの距離を取得

	// 自身の前方ベクトルを取得
	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); }
	Vector3 GetRight() const { return Vector3::Transform(Vector3::UnitY, mRotation); }
	
	Quaternion GetQuaternion() const { return mRotation; }

	void SetPitchSpeed(float speed) { mPitchSpeed = speed; } // ピッチの回転/秒速度を設定
	void SetMaxPitch(float pitch) { mMaxPitch = pitch; }     // 最大ピッチ角度を設定
	void SetDist(float dist) { mDist = dist; }               // 所有アクターとの距離を設定

private:
	Vector3 ComputeCameraPos() const;

	Vector3 mActualPos; // カメラの実際の位置
	Vector3 mVelocity;  // 実際のカメラの速度

	Quaternion mRotation; // 回転

	float mDist;          // 所有アクターとの距離
	float mTargetDist;    // 目標距離
	float mSpringConstant;// ばね定数（高いほど硬い）
	float mCameraZOffset;  // カメラ位置の上方向の補正値

	float mPitch;      // 現在のピッチ
	float mPitchSpeed; // ピッチの回転/秒速度
	float mMaxPitch;   // 最大ピッチ角度

	float mYaw;        // 現在のヨー
	float mYawSpeed;   // ヨーの回転/秒速度
};
