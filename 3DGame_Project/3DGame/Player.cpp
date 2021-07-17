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

	// メッシュの生成
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/Player.fbx"));
	mMeshComp->SetSkeleton(game->GetSkeleton("Assets/Player.fbx"));

	// アニメーションのロード
	mMeshComp->PlayAnimation(game->GetAnimation("Walk", "Assets/Player.fbx"), 1.0f);
	mMeshComp->PlayAnimation(game->GetAnimation("Idle", "Assets/Player.fbx"), 1.0f);
	
	// 他コンポーネントの生成
	mMoveComp = new PlayerMove(this);
	mMoveComp->SetIsFall(mIsGround);
	mCameraComp = new TPSCamera(this);
	
	// AABBの設定
	AABB box = mMeshComp->GetMesh()->GetBox();
	box.SetMinMax(PLAYER_WIDTH, PLAYER_DEPTH, PLAYER_HEIGHT);
	mBoxComp->SetObjectBox(box);
	mBoxComp->SetShouldRotate(false);
}

void Player::ActorInput(const struct InputState& state)
{
	// WASD移動
	InputDevice device;
	Vector2 leftAxis = state.GetMappedAxis("LeftAxis", device);

	mMoveComp->SetForward(mCameraComp->GetForward());
	mMoveComp->SetRight(mCameraComp->GetRight());
	Vector3 forward(leftAxis.y, leftAxis.x, 0);
	forward = Vector3::Transform(forward, mCameraComp->GetQuaternion());

	// プレイヤーの向き
	float angular = Vector3::Dot(forward, GetRight());
	mMoveComp->SetAngularSpeed(angular * 10);

	leftAxis *= 400.0f;
	mMoveComp->SetMoveSpeed(leftAxis);

	// アニメーション遷移
	if (leftAxis != Vector2::Zero && mCurrentState != PlayerState::Walk) ChangeWalk();
	else if(leftAxis == Vector2::Zero && mCurrentState != PlayerState::Idle) ChangeIdle();
	

	// ジャンプ
	// 着地判定
	bool result = CheckGround();

	// 着地している
	if (result)
	{
		// ジャンプ
		if (state.GetMappedButtonState("Jump") == ButtonState::EPressed)
		{
			mMoveComp->SetJumpSpeed(600.0f);
			mMoveComp->SetIsFall(true);
		}

		// 今回着地した
		if (mIsGround == false)
		{
			mMoveComp->SetJumpSpeed(0.0f);
			mMoveComp->SetIsFall(false);
			mIsGround = true;
		}
	}
	// 着地していない
	else
	{
		// 今回地面から離れた
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
	// 線分の作成
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