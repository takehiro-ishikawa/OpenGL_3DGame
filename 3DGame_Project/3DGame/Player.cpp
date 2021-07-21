#include "Player.h"
#include "PlayerState.h"
#include "Bullet.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "TPSCamera.h"
#include "PlayerMove.h"
#include "AudioComponent.h"
#include "BoxComponent.h"
#include "Bullet.h"
#include "SkeletalMeshComponent.h"
#include "PhysWorld.h"
#include "InputSystem.h"
#include <iostream>

Player::Player(Game* game)
	:Character(game)
	,mIsGround(false)
{
	SetScale(80.0f);
	SetPosition(GetPosition());

	// ���b�V���̐���
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh(PLAYER_FILEPATH));
	mMeshComp->SetSkeleton(game->GetSkeleton(PLAYER_FILEPATH));
	GetGame()->GetRenderer()->GetMesh(BULLET_FILEPATH);

	// �A�j���[�V�����̃��[�h
	mMeshComp->PlayAnimation(game->GetAnimation(PLAYER_ANIMATION_WALK, PLAYER_FILEPATH), 1.0f);
	mMeshComp->PlayAnimation(game->GetAnimation(PLAYER_ANIMATION_IDLE, PLAYER_FILEPATH), 1.0f);
	
	// ���R���|�[�l���g�̐���
	mMoveComp = new PlayerMove(this);
	mMoveComp->SetIsFall(mIsGround);
	mCameraComp = new TPSCamera(this);
	
	// �X�e�[�g�̐ݒ�
	RegisterState(new PlayerIdle(this));
	RegisterState(new PlayerWalk(this));
	ChangeState(PLAYER_IDLE);

	// AABB�̐ݒ�
	AABB box = mMeshComp->GetMesh()->GetBox();
	box.SetMinMax(PLAYER_WIDTH, PLAYER_DEPTH, PLAYER_HEIGHT);
	mBoxComp->SetObjectBox(box);
	mBoxComp->SetShouldRotate(false);
}

void Player::ActorInput(const struct InputState& state)
{
	// ���n����
	bool result = CheckGround();
	// ���n���Ă���
	if (result)
	{
		// ���񒅒n����
		if (mIsGround == false)
		{
			mMoveComp->SetJumpSpeed(0.0f);
			mMoveComp->SetIsFall(false);
			mIsGround = true;
		}
	}
	// ���n���Ă��Ȃ�
	else
	{
		// ����n�ʂ��痣�ꂽ
		if (mIsGround == true)
		{
			mMoveComp->SetIsFall(true);
			mIsGround = false;
		}
	}

	// ���݂̏�Ԃɉ��������͏���
	if (mCurrentState)
	{
		mCurrentState->Input(state);
	}
}

void Player::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	FixCollisions();

	if (mCurrentState)
	{
		mCurrentState->Update(deltaTime);
	}
}

void Player::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}

void Player::Shoot()
{
	// �J�n�_���擾
	Vector3 start = GetPosition();
	start.z += 150.0f;
	
	// �{�[�����X�|�[������
	Bullet * bullet = new Bullet(GetGame());
	bullet->SetPlayer(this);
	bullet->SetPosition(start + GetForward() * 20.0f);
	// �{�[������]�����ĐV��������������
	bullet->RotateToNewForward(GetForward());
}