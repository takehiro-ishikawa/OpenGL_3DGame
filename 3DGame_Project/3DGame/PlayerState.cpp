#include "StateMachine.h"
#include "Player.h"
#include "PlayerParameters.h"
#include "Game.h"
#include "Renderer.h"
#include "Character.h"
#include "InputSystem.h"
#include "SkeletalAnimation.h"
#include "CameraComponent.h"
#include "PlayerMove.h"
#include "Audio.h"
#include "Menu.h"


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
			mPlayer->Jump();
		}
	}

	// 状態遷移の確認
	InputDevice device;
	Vector2 leftAxis = state.GetMappedAxis(INPUT_LEFT_AXIS, device);
	// 近接攻撃キー入力で"近接攻撃"に遷移
	if (state.GetMappedButtonState(INPUT_ATTACK) == ButtonState::EPressed)
	{
		mPlayer->ChangeState(PLAYER_ATTACK);
	}
	// 移動キー入力
	if (leftAxis != Vector2::Zero)
	{
		// 射撃キー入力で"射撃&歩行"に遷移
		if (state.GetMappedButtonState(INPUT_FIRE_STANDBY) == ButtonState::EPressed) mPlayer->ChangeState(PLAYER_SHOOTWALK);
		// ダッシュキー入力で"ダッシュ"に遷移
	    else if (state.GetMappedButtonState(INPUT_SPRINT) == ButtonState::EHeld) mPlayer->ChangeState(PLAYER_RUN);
		// それ以外は"歩行"に遷移
		else mPlayer->ChangeState(PLAYER_WALK);
	}
	// 射撃準備キー入力
	else if (state.GetMappedButtonState(INPUT_FIRE_STANDBY) == ButtonState::EPressed)
	{
		mPlayer->GetAudioComp()->PlayEvent(SE_SHOOT_STANDBY);
		mPlayer->ChangeState(PLAYER_SHOOT);
	}
}

void PlayerIdle::Update(float deltaTime)
{

}

void PlayerIdle::OnEnter()
{
	mPlayer->GetMeshComp()->PlayAnimation(mPlayer->GetGame()->GetAnimation(PLAYER_ANIMATION_IDLE, PLAYER_FILEPATH), 1.0f);
	mPlayer->GetMoveComp()->SetMoveSpeed(Vector2::Zero);
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
			mPlayer->Jump();
		}
	}

	// 状態遷移の確認
	// 近接攻撃キー入力で"近接攻撃"に遷移
	if (state.GetMappedButtonState(INPUT_ATTACK) == ButtonState::EPressed)
	{
		mPlayer->ChangeState(PLAYER_ATTACK);
	}
	// 移動キー未入力
	if (leftAxis == Vector2::Zero)
	{
		// "待機"に遷移
		mPlayer->ChangeState(PLAYER_IDLE);
	}
	else if (state.GetMappedButtonState(INPUT_SPRINT) == ButtonState::EPressed)
	{
		// "ダッシュ"に遷移
		mPlayer->ChangeState(PLAYER_RUN);
	}
	// 射撃準備キー入力
	else if (state.GetMappedButtonState(INPUT_FIRE_STANDBY) == ButtonState::EPressed)
	{
		// "射撃&歩行"に遷移
		mPlayer->GetAudioComp()->PlayEvent(SE_SHOOT_STANDBY);
		mPlayer->ChangeState(PLAYER_SHOOTWALK);
	}
}

void PlayerWalk::Update(float deltaTime)
{

}

void PlayerWalk::OnEnter()
{
	mPlayer->GetMeshComp()->PlayAnimation(mPlayer->GetGame()->GetAnimation(PLAYER_ANIMATION_WALK, PLAYER_FILEPATH), 1.0f);
	mPlayer->PlayFootStepWalk(true);
}

void PlayerWalk::OnExit()
{
	mPlayer->PlayFootStepWalk(false);
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
			mPlayer->Jump();
		}
	}

	// 状態遷移の確認
	// 近接攻撃キー入力で"近接攻撃"に遷移
	if (state.GetMappedButtonState(INPUT_ATTACK) == ButtonState::EPressed)
	{
		mPlayer->ChangeState(PLAYER_ATTACK);
	}
	// 移動キー未入力
	if (leftAxis == Vector2::Zero)
	{
		// "待機"に遷移
		mPlayer->ChangeState(PLAYER_IDLE);
	}
	// ダッシュキー入力
	else if (state.GetMappedButtonState(INPUT_SPRINT) == ButtonState::EPressed)
	{
		// "歩行"に遷移
		mPlayer->ChangeState(PLAYER_WALK);
	}
	// 射撃準備キー入力
	else if (state.GetMappedButtonState(INPUT_FIRE_STANDBY) == ButtonState::EPressed)
	{
		// "射撃&歩行"に遷移
		mPlayer->GetAudioComp()->PlayEvent(SE_SHOOT_STANDBY);
		mPlayer->ChangeState(PLAYER_SHOOTWALK);
	}
}

void PlayerRun::Update(float deltaTime)
{

}

void PlayerRun::OnEnter()
{
	mPlayer->GetAudioComp()->PlayEvent(SE_DASH);
	mPlayer->GetMeshComp()->PlayAnimation(mPlayer->GetGame()->GetAnimation(PLAYER_ANIMATION_RUN, PLAYER_FILEPATH), 1.0f);
	mPlayer->PlayFootStepDash(true);
}

void PlayerRun::OnExit()
{
	mPlayer->PlayFootStepDash(false);
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
	// 近接攻撃キー入力で"近接攻撃"に遷移
	if (state.GetMappedButtonState(INPUT_ATTACK) == ButtonState::EPressed)
	{
		mPlayer->ChangeState(PLAYER_ATTACK);
	}
	// 移動キー入力
	if (leftAxis != Vector2::Zero)
	{
		// "射撃&歩行"に遷移
		mPlayer->ChangeState(PLAYER_SHOOTWALK);
	}
	// 射撃準備キーが放された
	else if (state.GetMappedButtonState(INPUT_FIRE_STANDBY) == ButtonState::ENone)
	{
		// "待機"に遷移
		mPlayer->ChangeState(PLAYER_IDLE);
	}
}

void PlayerShoot::Update(float deltaTime)
{

}

void PlayerShoot::OnEnter()
{
	mPlayer->GetCameraComp()->SetIdealDist(CAMERA_AIM_DIST);
	mPlayer->GetCameraComp()->SetIdealOffsetPos(CAMERA_AIM_OFFSETPOS);
	mPlayer->GetCameraComp()->SetMaxPitchSpeed(CAMERA_AIM_PITCHSPEED);
	mPlayer->GetCameraComp()->SetMaxYawSpeed(CAMERA_AIM_YAWSPEED);
	mPlayer->GetMeshComp()->PlayAnimation(mPlayer->GetGame()->GetAnimation(PLAYER_ANIMATION_SHOOT, PLAYER_FILEPATH), 1.0f);
}

void PlayerShoot::OnExit()
{
	mPlayer->GetCameraComp()->SetIdealDist(CAMERA_NORMAL_DIST);
	mPlayer->GetMoveComp()->SetAngularSpeed(0);
	mPlayer->GetCameraComp()->SetIdealOffsetPos(CAMERA_NORMAL_OFFSETPOS);
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
	// 近接攻撃キー入力で"近接攻撃"に遷移
	if (state.GetMappedButtonState(INPUT_ATTACK) == ButtonState::EPressed)
	{
		mPlayer->ChangeState(PLAYER_ATTACK);
	}
	// 射撃準備キーが放された
	if (state.GetMappedButtonState(INPUT_FIRE_STANDBY) == ButtonState::ENone)
	{
		// "待機"に遷移
		mPlayer->ChangeState(PLAYER_IDLE);
	}
	// 移動キー未入力
	else if (leftAxis == Vector2::Zero)
	{
		// "射撃"に遷移
		mPlayer->ChangeState(PLAYER_SHOOT);
	}
}

void PlayerShootWalk::Update(float deltaTime)
{

}

void PlayerShootWalk::OnEnter()
{
	mPlayer->GetCameraComp()->SetIdealDist(CAMERA_AIM_DIST);
	mPlayer->GetCameraComp()->SetIdealOffsetPos(CAMERA_AIM_OFFSETPOS);
	mPlayer->GetCameraComp()->SetMaxPitchSpeed(CAMERA_AIM_PITCHSPEED);
	mPlayer->GetCameraComp()->SetMaxYawSpeed(CAMERA_AIM_YAWSPEED);
	mPlayer->GetMeshComp()->PlayAnimation(mPlayer->GetGame()->GetAnimation(PLAYER_ANIMATION_SHOOT_WALK, PLAYER_FILEPATH), 1.0f);
	mPlayer->PlayFootStepWalk(true);
}

void PlayerShootWalk::OnExit()
{
	mPlayer->GetCameraComp()->SetIdealDist(CAMERA_NORMAL_DIST);
	mPlayer->GetMoveComp()->SetAngularSpeed(0);
	mPlayer->GetCameraComp()->SetIdealOffsetPos(CAMERA_NORMAL_OFFSETPOS);
	mPlayer->GetCameraComp()->SetMaxPitchSpeed(CAMERA_NORMAL_PITCHSPEED);
	mPlayer->GetCameraComp()->SetMaxYawSpeed(CAMERA_NORMAL_YAWSPEED);
	mPlayer->PlayFootStepWalk(false);
}

#pragma endregion


#pragma region "近接攻撃"状態

void PlayerAttack::Input(const struct InputState& state)
{

}

void PlayerAttack::Update(float deltaTime)
{
	if (mAttackTime > 0)
	{
		mAttackTime -= deltaTime;
		if (mAttackTime < 0) mPlayer->Attack();
	}

	mEndTime -= deltaTime;
	if (mEndTime < 0) mPlayer->ChangeState(PLAYER_IDLE);
}

void PlayerAttack::OnEnter()
{
	// 移動と回転は0にする
	mPlayer->GetMoveComp()->SetMoveSpeed(Vector2::Zero);
	mPlayer->GetMoveComp()->SetAngularSpeed(0);

	// 近接攻撃アニメーションとSE再生
	mPlayer->GetAudioComp()->PlayEvent(SE_SLASH);
	mPlayer->GetMeshComp()->PlayAnimation(mPlayer->GetGame()->GetAnimation(PLAYER_ANIMATION_ATTACK, PLAYER_FILEPATH), 1.0f);

	// 攻撃発生と終了のタイミングを設定
	mAttackTime = PLAYER_ATTACK_TIME;
	mEndTime = PLAYER_ATTACK_END_TIME;
}

void PlayerAttack::OnExit()
{

}

#pragma endregion


#pragma region "死亡"状態

void PlayerDead::Update(float deltaTime)
{
	mTimeCount -= deltaTime;
	
	// "死亡"状態に遷移後、設定した秒数経過でリザルト画面生成
	if (mTimeCount <= 0)
	{
		new ResultMenu(mPlayer->GetGame());
	}
}

void PlayerDead::OnEnter()
{
	mPlayer->GetMoveComp()->SetMoveSpeed(Vector2::Zero);
	mPlayer->GetMoveComp()->SetAngularSpeed(0);

	// アニメーション再生
	mPlayer->GetMeshComp()->PlayAnimation(mPlayer->GetGame()->GetAnimation(PLAYER_ANIMATION_DEAD, PLAYER_FILEPATH), 1.0f);
	// 時間設定
	mTimeCount = PLAYER_DEADTIME;
}

void PlayerDead::OnExit()
{

}

#pragma endregion
