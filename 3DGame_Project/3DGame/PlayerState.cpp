#include "PlayerState.h"
#include "Player.h"
#include "Game.h"
#include "Renderer.h"
#include "Character.h"
#include "InputSystem.h"
#include "SkeletalMeshComponent.h"
#include "TPSCamera.h"
#include "PlayerMove.h"

PlayerState::PlayerState(Character* owner)
	:State(owner)
{
	mPlayer = dynamic_cast<Player*>(owner);

	if (mPlayer == nullptr)
	{
		SDL_Log("Failed to get Player pointer");
	}
}

void PlayerIdle::Input(const struct InputState& state)
{
	// ジャンプ
	if (mPlayer->GetIsGround())
	{
		if (state.GetMappedButtonState("Jump") == ButtonState::EPressed)
		{
			mPlayer->GetMoveComp()->SetJumpSpeed(600.0f);
			mPlayer->GetMoveComp()->SetIsFall(true);
		}
	}

	// 射撃
	if (state.GetMappedButtonState("Fire") == ButtonState::EPressed)
	{
		mPlayer->Shoot();
	}

	// 移動入力で歩行状態への遷移
	InputDevice device;
	Vector2 leftAxis = state.GetMappedAxis("LeftAxis", device);
	if (leftAxis != Vector2::Zero)
	{
		mPlayer->ChangeState(PLAYER_WALK);
	}
}

void PlayerIdle::Update(float deltaTime)
{

}

void PlayerIdle::OnEnter()
{
	mPlayer->GetMeshComp()->PlayAnimation(mPlayer->GetGame()->GetAnimation("Idle", "Assets/Player.fbx"), 1.0f);
}

void PlayerIdle::OnExit()
{

}

void PlayerWalk::Input(const struct InputState& state)
{
	// WASD移動
	InputDevice device;
	Vector2 leftAxis = state.GetMappedAxis("LeftAxis", device);

	mPlayer->GetMoveComp()->SetForward(mPlayer->GetCameraComp()->GetForward());
	mPlayer->GetMoveComp()->SetRight(mPlayer->GetCameraComp()->GetRight());
	Vector3 forward(leftAxis.y, leftAxis.x, 0);
	forward = Vector3::Transform(forward, mPlayer->GetCameraComp()->GetQuaternion());

	// プレイヤーの向き
	float angular = Vector3::Dot(forward, mPlayer->GetRight());
	mPlayer->GetMoveComp()->SetAngularSpeed(angular * 10);

	leftAxis *= 400.0f;
	mPlayer->GetMoveComp()->SetMoveSpeed(leftAxis);

	// ジャンプ
	if (mPlayer->GetIsGround())
	{
		if (state.GetMappedButtonState("Jump") == ButtonState::EPressed)
		{
			mPlayer->GetMoveComp()->SetJumpSpeed(600.0f);
			mPlayer->GetMoveComp()->SetIsFall(true);
		}
	}

	// 射撃
	if (state.GetMappedButtonState("Fire") == ButtonState::EPressed)
	{
		mPlayer->Shoot();
	}

	// 移動入力無しで待機状態への遷移
	if (leftAxis == Vector2::Zero)
	{
		mPlayer->ChangeState(PLAYER_IDLE);
	}
}

void PlayerWalk::Update(float deltaTime)
{

}

void PlayerWalk::OnEnter()
{
	mPlayer->GetMeshComp()->PlayAnimation(mPlayer->GetGame()->GetAnimation("Walk", "Assets/Player.fbx"), 1.0f);
}

void PlayerWalk::OnExit()
{

}