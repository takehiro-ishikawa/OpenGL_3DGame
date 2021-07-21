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

	// メッシュの生成
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh(PLAYER_FILEPATH));
	mMeshComp->SetSkeleton(game->GetSkeleton(PLAYER_FILEPATH));
	GetGame()->GetRenderer()->GetMesh(BULLET_FILEPATH);

	// アニメーションのロード
	mMeshComp->PlayAnimation(game->GetAnimation(PLAYER_ANIMATION_WALK, PLAYER_FILEPATH), 1.0f);
	mMeshComp->PlayAnimation(game->GetAnimation(PLAYER_ANIMATION_IDLE, PLAYER_FILEPATH), 1.0f);
	
	// 他コンポーネントの生成
	mMoveComp = new PlayerMove(this);
	mMoveComp->SetIsFall(mIsGround);
	mCameraComp = new TPSCamera(this);
	
	// ステートの設定
	RegisterState(new PlayerIdle(this));
	RegisterState(new PlayerWalk(this));
	ChangeState(PLAYER_IDLE);

	// AABBの設定
	AABB box = mMeshComp->GetMesh()->GetBox();
	box.SetMinMax(PLAYER_WIDTH, PLAYER_DEPTH, PLAYER_HEIGHT);
	mBoxComp->SetObjectBox(box);
	mBoxComp->SetShouldRotate(false);
}

void Player::ActorInput(const struct InputState& state)
{
	// 着地判定
	bool result = CheckGround();
	// 着地している
	if (result)
	{
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

	// 現在の状態に応じた入力処理
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
	// 開始点を取得
	Vector3 start = GetPosition();
	start.z += 150.0f;
	
	// ボールをスポーンする
	Bullet * bullet = new Bullet(GetGame());
	bullet->SetPlayer(this);
	bullet->SetPosition(start + GetForward() * 20.0f);
	// ボールを回転させて新しい方向を向く
	bullet->RotateToNewForward(GetForward());
}