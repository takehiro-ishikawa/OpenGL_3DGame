#include "Player.h"
#include "PlayerState.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "TPSCamera.h"
#include "PlayerMove.h"
#include "AudioComponent.h"
#include "BoxComponent.h"
#include "Bullet.h"
#include "BulletMove.h"
#include "SkeletalMeshComponent.h"
#include "PhysWorld.h"
#include "InputSystem.h"
#include "HUD.h"
#include "PointLightComponent.h"
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
	GetGame()->GetRenderer()->GetMesh(PLAYERBULLET_FILEPATH);

	// アニメーションのロード
	mMeshComp->PlayAnimation(game->GetAnimation(PLAYER_ANIMATION_SHOOT, PLAYER_FILEPATH), 1.0f);
	mMeshComp->PlayAnimation(game->GetAnimation(PLAYER_ANIMATION_SHOOT_WALK, PLAYER_FILEPATH), 1.0f);
	mMeshComp->PlayAnimation(game->GetAnimation(PLAYER_ANIMATION_WALK, PLAYER_FILEPATH), 1.0f);
	mMeshComp->PlayAnimation(game->GetAnimation(PLAYER_ANIMATION_RUN, PLAYER_FILEPATH), 1.0f);
	mMeshComp->PlayAnimation(game->GetAnimation(PLAYER_ANIMATION_ATTACK, PLAYER_FILEPATH), 1.0f);
	mMeshComp->PlayAnimation(game->GetAnimation(PLAYER_ANIMATION_IDLE, PLAYER_FILEPATH), 1.0f);
	
	// 他コンポーネントの生成
	mMoveComp = new PlayerMove(this);
	mMoveComp->SetIsFall(mIsGround);
	mCameraComp = new TPSCamera(this);
	
	// ステートの設定
	RegisterState(new PlayerIdle(this));
	RegisterState(new PlayerWalk(this));
	RegisterState(new PlayerRun(this));
	RegisterState(new PlayerShoot(this));
	RegisterState(new PlayerShootWalk(this));
	RegisterState(new PlayerAttack(this));
	ChangeState(PLAYER_IDLE);

	// AABBの設定
	AABB box = mMeshComp->GetMesh()->GetBox();
	box.SetMinMax(PLAYER_WIDTH, PLAYER_DEPTH, PLAYER_HEIGHT);
	mBoxComp->SetObjectBox(box);
	mBoxComp->SetShouldRotate(false);

	// 足音のロード
	mWalk = mAudioComp->PlayEvent(SE_FOOTSTEP_WALK);
	mWalk.SetPaused(true);
	mDash = mAudioComp->PlayEvent(SE_FOOTSTEP_DASH);
	mDash.SetPaused(true);

	// 体力の設定
	mHealth = PLAYER_MAX_HEALTH;

	// タグの設定
	mTag = CharacterTag::EPlayer;

	// 中心座標の設定
	mCenterOffset = PLAYER_CENTER_OFFSET;
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

void Player::Damage(float value)
{
	// 体力からダメージを引く
	mHealth -= value;
	mHealth = Math::Max(mHealth, 0.0f);
	mHealth = Math::Min(mHealth, PLAYER_MAX_HEALTH);

	// 現在の体力を正規化してHUDに反映させる
	GetGame()->GetHUD()->SetNormalizeHealth(mHealth / PLAYER_MAX_HEALTH);

	// 体力が0になったら死亡
	if (mHealth <= 0)
	{
		Dead();
	}
}

void Player::Dead()
{

}

void Player::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}

void Player::Jump()
{
	mAudioComp->PlayEvent(SE_JUMP);
	mMoveComp->SetJumpSpeed(PLAYER_JUMP_SPEED);
	mMoveComp->SetIsFall(true);
}

void Player::Shoot()
{
	// 開始点を取得
	Vector3 start = GetPosition();
	Vector3 offset = PLAYER_SHOOT_POSITION_OFFSET;
	start += GetRight() * offset.x;
	start += GetForward() * offset.y;
	start.z += offset.z;
	
	// ボールをスポーンする
	Bullet * bullet = new Bullet(GetGame(), CharacterTag::EEnemy);
	bullet->GetMeshComp()->SetMesh(GetGame()->GetRenderer()->GetMesh(PLAYERBULLET_FILEPATH));
	bullet->GetMoveComp()->SetMoveSpeed(Vector2(0, PLAYER_BULLET_SPEED));
	bullet->GetPointLightComp()->mDiffuseColor = Color::Green;
	bullet->GetPointLightComp()->mOuterRadius = 200.0f;
	bullet->GetPointLightComp()->mInnerRadius = 100.0f;
	bullet->SetPlayer(this);
	bullet->SetPosition(start);
	// ボールを回転させて新しい方向を向く
	bullet->RotateToNewForward(GetForward());

	mAudioComp->PlayEvent(SE_SHOOT_P);
}

void Player::Attack()
{
	// 線分の作成
	Vector3 start = GetCenterPosition();
	Vector3 end = start + GetForward() * PLAYER_ATTACK_RANGE;

	LineSegment segment(start, end);

	PhysWorld* phys = GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;

	if (phys->SegmentCast(segment, info, this))
	{
		Character* target = dynamic_cast<Character*>(info.mActor);
		// キャラクターに当たった場合
		if (target)
		{
			// 当たったキャラクターが自身の標的ならダメージを与える
			target->Damage(10.0f);
		}
	}
}

void Player::PlayFootStepWalk(bool isPlay)
{
	// 再生
	if (isPlay)
	{
		if (!mWalk.GetPaused()) return;

		mWalk.SetPaused(!isPlay);
		mWalk.Restart();
	}
	// 停止
	else
	{
		mWalk.SetPaused(!isPlay);
	}
}

void Player::PlayFootStepDash(bool isPlay)
{
	// 再生
	if (isPlay)
	{
		if (!mDash.GetPaused()) return;

		mDash.SetPaused(!isPlay);
		mDash.Restart();
	}
	// 停止
	else
	{
		mDash.SetPaused(!isPlay);
	}
}