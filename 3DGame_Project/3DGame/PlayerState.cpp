#include "PlayerState.h"
#include "Game.h"
#include "Renderer.h"
#include "Character.h"
#include "InputSystem.h"
#include "SkeletalMeshComponent.h"
#include "TPSCamera.h"
#include "PlayerMove.h"
#include "AudioComponent.h"


PlayerState::PlayerState(Character* owner)
	:State(owner)
{
	mPlayer = dynamic_cast<Player*>(owner);

	if (mPlayer == nullptr)
	{
		SDL_Log("Failed to get Player pointer");
	}
}

#pragma region "待機"状態

void PlayerIdle::Input(const struct InputState& state)
{
	// ジャンプ
	if (mPlayer->GetIsGround())
	{
		if (state.GetMappedButtonState(INPUT_JUMP) == ButtonState::EPressed)
		{
			mPlayer->GetMoveComp()->SetJumpSpeed(PLAYER_JUMP_SPEED);
			mPlayer->GetMoveComp()->SetIsFall(true);
		}
	}

	// 状態遷移の確認
	InputDevice device;
	Vector2 leftAxis = state.GetMappedAxis(INPUT_LEFT_AXIS, device);
	// 移動キー入力
	if (leftAxis != Vector2::Zero)
	{
		// 射撃キー入力で"射撃&歩行"に遷移
		if (state.GetMappedButtonState(INPUT_FIRE_STANDBY) == ButtonState::EPressed) mPlayer->ChangeState(PLAYER_ANIMATION_SHOOT_WALK);
		// ダッシュキー入力で"ダッシュ"に遷移
	    else if (state.GetMappedButtonState(INPUT_SPRINT) == ButtonState::EHeld) mPlayer->ChangeState(PLAYER_ANIMATION_RUN);
		// それ以外は"歩行"に遷移
		else mPlayer->ChangeState(PLAYER_ANIMATION_WALK);
	}
	// 射撃準備キー入力
	else if (state.GetMappedButtonState(INPUT_FIRE_STANDBY) == ButtonState::EPressed)
	{
		mPlayer->GetAudioComp()->PlayEvent("event:/ShootStandBy");
		mPlayer->ChangeState(PLAYER_ANIMATION_SHOOT);
	}
}

void PlayerIdle::Update(float deltaTime)
{

}

void PlayerIdle::OnEnter()
{
	mPlayer->GetMeshComp()->PlayAnimation(mPlayer->GetGame()->GetAnimation(PLAYER_ANIMATION_IDLE, PLAYER_FILEPATH), 1.0f);
}

void PlayerIdle::OnExit()
{

}

#pragma endregion


#pragma region "歩行"状態

void PlayerWalk::Input(const struct InputState& state)
{
	// 移動キー入力値を取得
	InputDevice device;
	Vector2 leftAxis = state.GetMappedAxis(INPUT_LEFT_AXIS, device);

	// プレイヤーの回転
	mPlayer->GetMoveComp()->SetForward(mPlayer->GetCameraComp()->GetForward());
	mPlayer->GetMoveComp()->SetRight(mPlayer->GetCameraComp()->GetRight());
	Vector3 forward(leftAxis.y, leftAxis.x, 0);
	forward = Vector3::Transform(forward, mPlayer->GetCameraComp()->GetQuaternion());
	float angular = Vector3::Dot(forward, mPlayer->GetRight());
	mPlayer->GetMoveComp()->SetAngularSpeed(angular * PLAYER_ROTATE_SPEED);

	// WASD移動
	leftAxis *= PLAYER_WALK_SPEED;
	mPlayer->GetMoveComp()->SetMoveSpeed(leftAxis);

	// ジャンプ
	if (mPlayer->GetIsGround())
	{
		if (state.GetMappedButtonState(INPUT_JUMP) == ButtonState::EPressed)
		{
			mPlayer->GetMoveComp()->SetJumpSpeed(PLAYER_JUMP_SPEED);
			mPlayer->GetMoveComp()->SetIsFall(true);
		}
	}

	// 状態遷移の確認
	// 移動キー未入力
	if (leftAxis == Vector2::Zero)
	{
		// "待機"に遷移
		mPlayer->ChangeState(PLAYER_ANIMATION_IDLE);
	}
	else if (state.GetMappedButtonState(INPUT_SPRINT) == ButtonState::EPressed)
	{
		// "ダッシュ"に遷移
		mPlayer->ChangeState(PLAYER_ANIMATION_RUN);
	}
	// 射撃準備キー入力
	else if (state.GetMappedButtonState(INPUT_FIRE_STANDBY) == ButtonState::EPressed)
	{
		// "射撃&歩行"に遷移
		mPlayer->GetAudioComp()->PlayEvent("event:/ShootStandBy");
		mPlayer->ChangeState(PLAYER_ANIMATION_SHOOT_WALK);
	}
}

void PlayerWalk::Update(float deltaTime)
{

}

void PlayerWalk::OnEnter()
{
	mPlayer->GetMeshComp()->PlayAnimation(mPlayer->GetGame()->GetAnimation(PLAYER_ANIMATION_WALK, PLAYER_FILEPATH), 1.0f);
}

void PlayerWalk::OnExit()
{

}

#pragma endregion


#pragma region "ダッシュ"状態

void PlayerRun::Input(const struct InputState& state)
{
	// 移動キー入力値を取得
	InputDevice device;
	Vector2 leftAxis = state.GetMappedAxis(INPUT_LEFT_AXIS, device);

	// プレイヤーの回転
	mPlayer->GetMoveComp()->SetForward(mPlayer->GetCameraComp()->GetForward());
	mPlayer->GetMoveComp()->SetRight(mPlayer->GetCameraComp()->GetRight());
	Vector3 forward(leftAxis.y, leftAxis.x, 0);
	forward = Vector3::Transform(forward, mPlayer->GetCameraComp()->GetQuaternion());
	float angular = Vector3::Dot(forward, mPlayer->GetRight());
	mPlayer->GetMoveComp()->SetAngularSpeed(angular * PLAYER_ROTATE_SPEED);

	// WASD移動
	leftAxis *= PLAYER_RUN_SPEED;
	mPlayer->GetMoveComp()->SetMoveSpeed(leftAxis);

	// ジャンプ
	if (mPlayer->GetIsGround())
	{
		if (state.GetMappedButtonState(INPUT_JUMP) == ButtonState::EPressed)
		{
			mPlayer->GetMoveComp()->SetJumpSpeed(PLAYER_JUMP_SPEED);
			mPlayer->GetMoveComp()->SetIsFall(true);
		}
	}

	// 状態遷移の確認
	// 移動キー未入力
	if (leftAxis == Vector2::Zero)
	{
		// "待機"に遷移
		mPlayer->ChangeState(PLAYER_ANIMATION_IDLE);
	}
	// ダッシュキー入力
	else if (state.GetMappedButtonState(INPUT_SPRINT) == ButtonState::EPressed)
	{
		// "歩行"に遷移
		mPlayer->ChangeState(PLAYER_ANIMATION_WALK);
	}
	// 射撃準備キー入力
	else if (state.GetMappedButtonState(INPUT_FIRE_STANDBY) == ButtonState::EPressed)
	{
		// "射撃&歩行"に遷移
		mPlayer->GetAudioComp()->PlayEvent("event:/ShootStandBy");
		mPlayer->ChangeState(PLAYER_ANIMATION_SHOOT_WALK);
	}
}

void PlayerRun::Update(float deltaTime)
{

}

void PlayerRun::OnEnter()
{
	mPlayer->GetMeshComp()->PlayAnimation(mPlayer->GetGame()->GetAnimation(PLAYER_ANIMATION_RUN, PLAYER_FILEPATH), 1.0f);
}

void PlayerRun::OnExit()
{

}

#pragma endregion


#pragma region "射撃"状態

void PlayerShoot::Input(const struct InputState& state)
{
	// 射撃
	if (state.GetMappedButtonState(INPUT_FIRE) == ButtonState::EPressed)
	{
		mPlayer->Shoot();
	}

	// プレイヤーの回転
	Vector3 forward = Vector3::UnitX;
	forward = Vector3::Transform(forward, mPlayer->GetCameraComp()->GetQuaternion());
	float angular = Vector3::Dot(forward, mPlayer->GetRight());
	mPlayer->GetMoveComp()->SetAngularSpeed(angular * PLAYER_ROTATE_SPEED);

	// 状態遷移の確認
	InputDevice device;
	Vector2 leftAxis = state.GetMappedAxis(INPUT_LEFT_AXIS, device);
	// 移動キー入力
	if (leftAxis != Vector2::Zero)
	{
		// "射撃&歩行"に遷移
		mPlayer->ChangeState(PLAYER_ANIMATION_SHOOT_WALK);
	}
	// 射撃準備キーが放された
	else if (state.GetMappedButtonState(INPUT_FIRE_STANDBY) == ButtonState::EReleased)
	{
		// "待機"に遷移
		mPlayer->ChangeState(PLAYER_ANIMATION_IDLE);
	}
}

void PlayerShoot::Update(float deltaTime)
{

}

void PlayerShoot::OnEnter()
{
	mPlayer->GetCameraComp()->SetDist(CAMERA_AIM_DIST);
	mPlayer->GetCameraComp()->SetOffsetPos(CAMERA_AIM_OFFSETPOS);
	mPlayer->GetCameraComp()->SetMaxPitchSpeed(CAMERA_AIM_PITCHSPEED);
	mPlayer->GetCameraComp()->SetMaxYawSpeed(CAMERA_AIM_YAWSPEED);
	mPlayer->GetMeshComp()->PlayAnimation(mPlayer->GetGame()->GetAnimation(PLAYER_ANIMATION_SHOOT, PLAYER_FILEPATH), 1.0f);
}

void PlayerShoot::OnExit()
{
	mPlayer->GetCameraComp()->SetDist(CAMERA_NORMAL_DIST);
	mPlayer->GetMoveComp()->SetAngularSpeed(0);
	mPlayer->GetCameraComp()->SetOffsetPos(CAMERA_NORMAL_OFFSETPOS);
	mPlayer->GetCameraComp()->SetMaxPitchSpeed(CAMERA_NORMAL_PITCHSPEED);
	mPlayer->GetCameraComp()->SetMaxYawSpeed(CAMERA_NORMAL_YAWSPEED);
}

#pragma endregion


#pragma region "射撃&歩行"状態

void PlayerShootWalk::Input(const struct InputState& state)
{
	// 移動キー入力値を取得
	InputDevice device;
	Vector2 leftAxis = state.GetMappedAxis(INPUT_LEFT_AXIS, device);

	// プレイヤーの回転
	// プレイヤーの回転
	mPlayer->GetMoveComp()->SetForward(mPlayer->GetCameraComp()->GetForward());
	mPlayer->GetMoveComp()->SetRight(mPlayer->GetCameraComp()->GetRight());
	Vector3 forward = Vector3::UnitX;
	forward = Vector3::Transform(forward, mPlayer->GetCameraComp()->GetQuaternion());
	float angular = Vector3::Dot(forward, mPlayer->GetRight());
	mPlayer->GetMoveComp()->SetAngularSpeed(angular * PLAYER_ROTATE_SPEED);

	// WASD移動
	leftAxis *= PLAYER_WALK_SPEED;
	mPlayer->GetMoveComp()->SetMoveSpeed(leftAxis);

	// 射撃
	if (state.GetMappedButtonState(INPUT_FIRE) == ButtonState::EPressed)
	{
		mPlayer->Shoot();
	}

	// 状態遷移の確認
	// 射撃準備キーが放された
	if (state.GetMappedButtonState(INPUT_FIRE_STANDBY) == ButtonState::EReleased)
	{
		// "待機"に遷移
		mPlayer->ChangeState(PLAYER_ANIMATION_IDLE);
	}
	// 移動キー未入力
	else if (leftAxis == Vector2::Zero)
	{
		// "射撃"に遷移
		mPlayer->ChangeState(PLAYER_ANIMATION_SHOOT);
	}
}

void PlayerShootWalk::Update(float deltaTime)
{

}

void PlayerShootWalk::OnEnter()
{
	mPlayer->GetCameraComp()->SetDist(CAMERA_AIM_DIST);
	mPlayer->GetCameraComp()->SetOffsetPos(CAMERA_AIM_OFFSETPOS);
	mPlayer->GetCameraComp()->SetMaxPitchSpeed(CAMERA_AIM_PITCHSPEED);
	mPlayer->GetCameraComp()->SetMaxYawSpeed(CAMERA_AIM_YAWSPEED);
	mPlayer->GetMeshComp()->PlayAnimation(mPlayer->GetGame()->GetAnimation(PLAYER_ANIMATION_SHOOT_WALK, PLAYER_FILEPATH), 1.0f);
}

void PlayerShootWalk::OnExit()
{
	mPlayer->GetCameraComp()->SetDist(CAMERA_NORMAL_DIST);
	mPlayer->GetMoveComp()->SetAngularSpeed(0);
	mPlayer->GetCameraComp()->SetOffsetPos(CAMERA_NORMAL_OFFSETPOS);
	mPlayer->GetCameraComp()->SetMaxPitchSpeed(CAMERA_NORMAL_PITCHSPEED);
	mPlayer->GetCameraComp()->SetMaxYawSpeed(CAMERA_NORMAL_YAWSPEED);
}

#pragma endregion