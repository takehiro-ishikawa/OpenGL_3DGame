#include "Player.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "TPSCamera.h"
#include "PlayerMove.h"
#include "BoxComponent.h"
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
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/Player.fbx"));
	mMeshComp->SetSkeleton(game->GetSkeleton("Assets/Player.fbx"));

	// �A�j���[�V�����̃��[�h
	mMeshComp->PlayAnimation(game->GetAnimation("Walk", "Assets/Player.fbx"), 1.0f);
	mMeshComp->PlayAnimation(game->GetAnimation("Idle", "Assets/Player.fbx"), 1.0f);
	
	// ���R���|�[�l���g�̐���
	mMoveComp = new PlayerMove(this);
	mMoveComp->SetIsFall(mIsGround);
	mCameraComp = new TPSCamera(this);
	
	// AABB�̐ݒ�
	AABB box = mMeshComp->GetMesh()->GetBox();
	box.SetMinMax(PLAYER_WIDTH, PLAYER_DEPTH, PLAYER_HEIGHT);
	mBoxComp->SetObjectBox(box);
	mBoxComp->SetShouldRotate(false);
}

void Player::ActorInput(const struct InputState& state)
{
	// WASD�ړ�
	InputDevice device;
	Vector2 leftAxis = state.GetMappedAxis("LeftAxis", device);

	mMoveComp->SetForward(mCameraComp->GetForward());
	mMoveComp->SetRight(mCameraComp->GetRight());
	Vector3 forward(leftAxis.y, leftAxis.x, 0);
	forward = Vector3::Transform(forward, mCameraComp->GetQuaternion());

	// �v���C���[�̌���
	float angular = Vector3::Dot(forward, GetRight());
	mMoveComp->SetAngularSpeed(angular * 10);

	leftAxis *= 400.0f;
	mMoveComp->SetMoveSpeed(leftAxis);

	// �A�j���[�V�����J��
	if (leftAxis != Vector2::Zero && mCurrentState != PlayerState::Walk) ChangeWalk();
	else if(leftAxis == Vector2::Zero && mCurrentState != PlayerState::Idle) ChangeIdle();
	

	// �W�����v
	// ���n����
	bool result = CheckGround();

	// ���n���Ă���
	if (result)
	{
		// �W�����v
		if (state.GetMappedButtonState("Jump") == ButtonState::EPressed)
		{
			mMoveComp->SetJumpSpeed(600.0f);
			mMoveComp->SetIsFall(true);
		}

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
}

void Player::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	FixCollisions();
}

void Player::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}

bool Player::CheckGround()
{
	// �����̍쐬
	float segmentLength = 2.0f;
	Vector3 start = GetPosition();
	start.z += 1.0f;
	Vector3 end = start + Vector3::UnitZ * -segmentLength;

	LineSegment segment(start, end);

	PhysWorld* phys = GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;

	return phys->SegmentCast(segment, info, this);
}

void Player::ChangeIdle()
{
	mCurrentState = PlayerState::Idle;
	mMeshComp->PlayAnimation(GetGame()->GetAnimation("Idle", "Assets/Player.fbx"), 1.0f);
}

void Player::ChangeWalk()
{
	mCurrentState = PlayerState::Walk;
	mMeshComp->PlayAnimation(GetGame()->GetAnimation("Walk", "Assets/Player.fbx"), 1.0f);
}