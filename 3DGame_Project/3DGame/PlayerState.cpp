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

#pragma region "�ҋ@"���

void PlayerIdle::Input(const struct InputState& state)
{
	// �W�����v
	if (mPlayer->GetIsGround())
	{
		if (state.GetMappedButtonState(INPUT_JUMP) == ButtonState::EPressed)
		{
			mPlayer->Jump();
		}
	}

	// ��ԑJ�ڂ̊m�F
	InputDevice device;
	Vector2 leftAxis = state.GetMappedAxis(INPUT_LEFT_AXIS, device);
	// �ߐڍU���L�[���͂�"�ߐڍU��"�ɑJ��
	if (state.GetMappedButtonState(INPUT_ATTACK) == ButtonState::EPressed)
	{
		mPlayer->ChangeState(PLAYER_ATTACK);
	}
	// �ړ��L�[����
	if (leftAxis != Vector2::Zero)
	{
		// �ˌ��L�[���͂�"�ˌ�&���s"�ɑJ��
		if (state.GetMappedButtonState(INPUT_FIRE_STANDBY) == ButtonState::EPressed) mPlayer->ChangeState(PLAYER_SHOOTWALK);
		// �_�b�V���L�[���͂�"�_�b�V��"�ɑJ��
	    else if (state.GetMappedButtonState(INPUT_SPRINT) == ButtonState::EHeld) mPlayer->ChangeState(PLAYER_RUN);
		// ����ȊO��"���s"�ɑJ��
		else mPlayer->ChangeState(PLAYER_WALK);
	}
	// �ˌ������L�[����
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


#pragma region "���s"���

void PlayerWalk::Input(const struct InputState& state)
{
	// �ړ��L�[���͒l���擾
	InputDevice device;
	Vector2 leftAxis = state.GetMappedAxis(INPUT_LEFT_AXIS, device);

	// �v���C���[�̉�]
	mPlayer->GetMoveComp()->SetForward(mPlayer->GetCameraComp()->GetForward());
	mPlayer->GetMoveComp()->SetRight(mPlayer->GetCameraComp()->GetRight());
	Vector3 forward(leftAxis.y, leftAxis.x, 0);
	forward = Vector3::Transform(forward, mPlayer->GetCameraComp()->GetQuaternion());
	float angular = Vector3::Dot(forward, mPlayer->GetRight());
	mPlayer->GetMoveComp()->SetAngularSpeed(angular * PLAYER_ROTATE_SPEED);

	// WASD�ړ�
	leftAxis *= PLAYER_WALK_SPEED;
	mPlayer->GetMoveComp()->SetMoveSpeed(leftAxis);

	// �W�����v
	if (mPlayer->GetIsGround())
	{
		if (state.GetMappedButtonState(INPUT_JUMP) == ButtonState::EPressed)
		{
			mPlayer->Jump();
		}
	}

	// ��ԑJ�ڂ̊m�F
	// �ߐڍU���L�[���͂�"�ߐڍU��"�ɑJ��
	if (state.GetMappedButtonState(INPUT_ATTACK) == ButtonState::EPressed)
	{
		mPlayer->ChangeState(PLAYER_ATTACK);
	}
	// �ړ��L�[������
	if (leftAxis == Vector2::Zero)
	{
		// "�ҋ@"�ɑJ��
		mPlayer->ChangeState(PLAYER_IDLE);
	}
	else if (state.GetMappedButtonState(INPUT_SPRINT) == ButtonState::EPressed)
	{
		// "�_�b�V��"�ɑJ��
		mPlayer->ChangeState(PLAYER_RUN);
	}
	// �ˌ������L�[����
	else if (state.GetMappedButtonState(INPUT_FIRE_STANDBY) == ButtonState::EPressed)
	{
		// "�ˌ�&���s"�ɑJ��
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


#pragma region "�_�b�V��"���

void PlayerRun::Input(const struct InputState& state)
{
	// �ړ��L�[���͒l���擾
	InputDevice device;
	Vector2 leftAxis = state.GetMappedAxis(INPUT_LEFT_AXIS, device);

	// �v���C���[�̉�]
	mPlayer->GetMoveComp()->SetForward(mPlayer->GetCameraComp()->GetForward());
	mPlayer->GetMoveComp()->SetRight(mPlayer->GetCameraComp()->GetRight());
	Vector3 forward(leftAxis.y, leftAxis.x, 0);
	forward = Vector3::Transform(forward, mPlayer->GetCameraComp()->GetQuaternion());
	float angular = Vector3::Dot(forward, mPlayer->GetRight());
	mPlayer->GetMoveComp()->SetAngularSpeed(angular * PLAYER_ROTATE_SPEED);

	// WASD�ړ�
	leftAxis *= PLAYER_RUN_SPEED;
	mPlayer->GetMoveComp()->SetMoveSpeed(leftAxis);

	// �W�����v
	if (mPlayer->GetIsGround())
	{
		if (state.GetMappedButtonState(INPUT_JUMP) == ButtonState::EPressed)
		{
			mPlayer->Jump();
		}
	}

	// ��ԑJ�ڂ̊m�F
	// �ߐڍU���L�[���͂�"�ߐڍU��"�ɑJ��
	if (state.GetMappedButtonState(INPUT_ATTACK) == ButtonState::EPressed)
	{
		mPlayer->ChangeState(PLAYER_ATTACK);
	}
	// �ړ��L�[������
	if (leftAxis == Vector2::Zero)
	{
		// "�ҋ@"�ɑJ��
		mPlayer->ChangeState(PLAYER_IDLE);
	}
	// �_�b�V���L�[����
	else if (state.GetMappedButtonState(INPUT_SPRINT) == ButtonState::EPressed)
	{
		// "���s"�ɑJ��
		mPlayer->ChangeState(PLAYER_WALK);
	}
	// �ˌ������L�[����
	else if (state.GetMappedButtonState(INPUT_FIRE_STANDBY) == ButtonState::EPressed)
	{
		// "�ˌ�&���s"�ɑJ��
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


#pragma region "�ˌ�"���

void PlayerShoot::Input(const struct InputState& state)
{
	// �ˌ�
	if (state.GetMappedButtonState(INPUT_FIRE) == ButtonState::EPressed)
	{
		mPlayer->Shoot();
	}

	// �v���C���[�̉�]
	Vector3 forward = Vector3::UnitX;
	forward = Vector3::Transform(forward, mPlayer->GetCameraComp()->GetQuaternion());
	float angular = Vector3::Dot(forward, mPlayer->GetRight());
	mPlayer->GetMoveComp()->SetAngularSpeed(angular * PLAYER_ROTATE_SPEED);

	// ��ԑJ�ڂ̊m�F
	InputDevice device;
	Vector2 leftAxis = state.GetMappedAxis(INPUT_LEFT_AXIS, device);
	// �ߐڍU���L�[���͂�"�ߐڍU��"�ɑJ��
	if (state.GetMappedButtonState(INPUT_ATTACK) == ButtonState::EPressed)
	{
		mPlayer->ChangeState(PLAYER_ATTACK);
	}
	// �ړ��L�[����
	if (leftAxis != Vector2::Zero)
	{
		// "�ˌ�&���s"�ɑJ��
		mPlayer->ChangeState(PLAYER_SHOOTWALK);
	}
	// �ˌ������L�[�������ꂽ
	else if (state.GetMappedButtonState(INPUT_FIRE_STANDBY) == ButtonState::ENone)
	{
		// "�ҋ@"�ɑJ��
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


#pragma region "�ˌ�&���s"���

void PlayerShootWalk::Input(const struct InputState& state)
{
	// �ړ��L�[���͒l���擾
	InputDevice device;
	Vector2 leftAxis = state.GetMappedAxis(INPUT_LEFT_AXIS, device);

	// �v���C���[�̉�]
	// �v���C���[�̉�]
	mPlayer->GetMoveComp()->SetForward(mPlayer->GetCameraComp()->GetForward());
	mPlayer->GetMoveComp()->SetRight(mPlayer->GetCameraComp()->GetRight());
	Vector3 forward = Vector3::UnitX;
	forward = Vector3::Transform(forward, mPlayer->GetCameraComp()->GetQuaternion());
	float angular = Vector3::Dot(forward, mPlayer->GetRight());
	mPlayer->GetMoveComp()->SetAngularSpeed(angular * PLAYER_ROTATE_SPEED);

	// WASD�ړ�
	leftAxis *= PLAYER_WALK_SPEED;
	mPlayer->GetMoveComp()->SetMoveSpeed(leftAxis);

	// �ˌ�
	if (state.GetMappedButtonState(INPUT_FIRE) == ButtonState::EPressed)
	{
		mPlayer->Shoot();
	}

	// ��ԑJ�ڂ̊m�F
	// �ߐڍU���L�[���͂�"�ߐڍU��"�ɑJ��
	if (state.GetMappedButtonState(INPUT_ATTACK) == ButtonState::EPressed)
	{
		mPlayer->ChangeState(PLAYER_ATTACK);
	}
	// �ˌ������L�[�������ꂽ
	if (state.GetMappedButtonState(INPUT_FIRE_STANDBY) == ButtonState::ENone)
	{
		// "�ҋ@"�ɑJ��
		mPlayer->ChangeState(PLAYER_IDLE);
	}
	// �ړ��L�[������
	else if (leftAxis == Vector2::Zero)
	{
		// "�ˌ�"�ɑJ��
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


#pragma region "�ߐڍU��"���

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
	// �ړ��Ɖ�]��0�ɂ���
	mPlayer->GetMoveComp()->SetMoveSpeed(Vector2::Zero);
	mPlayer->GetMoveComp()->SetAngularSpeed(0);

	// �ߐڍU���A�j���[�V������SE�Đ�
	mPlayer->GetAudioComp()->PlayEvent(SE_SLASH);
	mPlayer->GetMeshComp()->PlayAnimation(mPlayer->GetGame()->GetAnimation(PLAYER_ANIMATION_ATTACK, PLAYER_FILEPATH), 1.0f);

	// �U�������ƏI���̃^�C�~���O��ݒ�
	mAttackTime = PLAYER_ATTACK_TIME;
	mEndTime = PLAYER_ATTACK_END_TIME;
}

void PlayerAttack::OnExit()
{

}

#pragma endregion


#pragma region "���S"���

void PlayerDead::Update(float deltaTime)
{
	mTimeCount -= deltaTime;
	
	// "���S"��ԂɑJ�ڌ�A�ݒ肵���b���o�߂Ń��U���g��ʐ���
	if (mTimeCount <= 0)
	{
		new ResultMenu(mPlayer->GetGame());
	}
}

void PlayerDead::OnEnter()
{
	mPlayer->GetMoveComp()->SetMoveSpeed(Vector2::Zero);
	mPlayer->GetMoveComp()->SetAngularSpeed(0);

	// �A�j���[�V�����Đ�
	mPlayer->GetMeshComp()->PlayAnimation(mPlayer->GetGame()->GetAnimation(PLAYER_ANIMATION_DEAD, PLAYER_FILEPATH), 1.0f);
	// ���Ԑݒ�
	mTimeCount = PLAYER_DEADTIME;
}

void PlayerDead::OnExit()
{

}

#pragma endregion
