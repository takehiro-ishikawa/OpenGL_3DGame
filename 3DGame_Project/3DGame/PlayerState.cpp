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
	// �W�����v
	if (mPlayer->GetIsGround())
	{
		if (state.GetMappedButtonState("Jump") == ButtonState::EPressed)
		{
			mPlayer->GetMoveComp()->SetJumpSpeed(600.0f);
			mPlayer->GetMoveComp()->SetIsFall(true);
		}
	}

	// �ˌ�
	if (state.GetMappedButtonState("Fire") == ButtonState::EPressed)
	{
		mPlayer->Shoot();
	}

	// �ړ����͂ŕ��s��Ԃւ̑J��
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
	// WASD�ړ�
	InputDevice device;
	Vector2 leftAxis = state.GetMappedAxis("LeftAxis", device);

	mPlayer->GetMoveComp()->SetForward(mPlayer->GetCameraComp()->GetForward());
	mPlayer->GetMoveComp()->SetRight(mPlayer->GetCameraComp()->GetRight());
	Vector3 forward(leftAxis.y, leftAxis.x, 0);
	forward = Vector3::Transform(forward, mPlayer->GetCameraComp()->GetQuaternion());

	// �v���C���[�̌���
	float angular = Vector3::Dot(forward, mPlayer->GetRight());
	mPlayer->GetMoveComp()->SetAngularSpeed(angular * 10);

	leftAxis *= 400.0f;
	mPlayer->GetMoveComp()->SetMoveSpeed(leftAxis);

	// �W�����v
	if (mPlayer->GetIsGround())
	{
		if (state.GetMappedButtonState("Jump") == ButtonState::EPressed)
		{
			mPlayer->GetMoveComp()->SetJumpSpeed(600.0f);
			mPlayer->GetMoveComp()->SetIsFall(true);
		}
	}

	// �ˌ�
	if (state.GetMappedButtonState("Fire") == ButtonState::EPressed)
	{
		mPlayer->Shoot();
	}

	// �ړ����͖����őҋ@��Ԃւ̑J��
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