#pragma once
#include "CameraComponent.h"
#include "Math.h"

#define CAMERA_MAX_ROTATE_SPEED 500.0f // カメラの最大回転速度
#define INPUT_CORRECTION_VALUE  20.0f  // キーボードとコントローラ入力の補正値

// プロトタイプ宣言
class Actor;

class TPSCamera : public CameraComponent
{
public:
	TPSCamera(Actor* owner);

	void ProcessInput(const struct InputState& state) override;
	void Update(float deltaTime) override;

	void SetTargetDist(float dist) { mTargetDist = dist; }
	void SetSpringConstant(float spring) { mSpringConstant = spring; }

	// ゲッター/セッター
	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); } // 自身の前方ベクトルを取得
	Vector3 GetRight() const { return Vector3::Transform(Vector3::UnitY, mRotation); }
	Quaternion GetQuaternion() const { return mRotation; }

	float GetPitch() const { return mPitch; }                      // 現在のピッチを取得
	float GetPitchSpeed() const { return mPitchSpeed; }            // ピッチの回転/秒速度を取得
	float GetMaxPitch() const { return mMaxPitch; }                // 最大ピッチ角度を取得
	Vector3 GetIdealOffsetPos() const { return mIdealOffsetPos; }  // 位置の理想のオフセット値を取得
	float GetIdealDist() const { return mIdealCameraDist; }        // 所有アクターとの理想の距離を取得
	float GetMaxYawSpeed() const { return mMaxYawSpeed; }          // ヨーの最大回転速度を取得
	float GetMaxPitchSpeed() const { return mMaxPitchSpeed; }      // ピッチの最大回転速度を取得

	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }       // ピッチの回転/秒速度を設定
	void SetMaxPitch(float pitch) { mMaxPitch = pitch; }           // 最大ピッチ角度を設定
	void SetIdealOffsetPos(Vector3 value) { mIdealOffsetPos = value; }       // 位置のオフセット値を設定
	void SetIdealDist(float dist) { mIdealCameraDist = dist; }               // 所有アクターとの距離を設定
	void SetMaxYawSpeed(float speed) { mMaxYawSpeed = speed; }     // ヨーの最大回転速度を設定
	void SetMaxPitchSpeed(float speed) { mMaxPitchSpeed = speed; } // ピッチの最大回転速度を設定

private:
	void ComputeCameraPos(float value);

	Vector3 mActualPos;    // カメラの実際の位置
	Vector3 mVelocity;     // 実際のカメラの速度
	Quaternion mRotation;  // カメラの回転

	Vector3 mIdealOffsetPos;    // 位置の理想のオフセット値
	Vector3 mActualOffsetPos;   // 位置の実際のオフセット値
	float mIdealCameraDist;     // アクターから離す理想の距離
	float mActualCameraDist;    // アクターから離す実際の距離
	float mTargetDist;     // 目標距離
	float mSpringConstant; // ばね定数（高いほど硬い）

	float mPitch;          // 現在のピッチ
	float mPitchSpeed;     // ピッチの回転/秒速度
	float mMaxPitch;       // 最大ピッチ角度
	float mMaxPitchSpeed;  // ピッチの最大回転速度

	float mYaw;            // 現在のヨー
	float mYawSpeed;       // ヨーの回転/秒速度
	float mMaxYawSpeed;    // ヨーの最大回転速度
};
