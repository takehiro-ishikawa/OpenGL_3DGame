#include "Player.h"
#include "PlayerParameters.h"
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

	// ���b�V���̐���
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh(PLAYER_FILEPATH));
	mMeshComp->SetSkeleton(game->GetSkeleton(PLAYER_FILEPATH));
	GetGame()->GetRenderer()->GetMesh(PLAYERBULLET_FILEPATH);

	// �A�j���[�V�����̃��[�h
	mMeshComp->PlayAnimation(game->GetAnimation(PLAYER_ANIMATION_SHOOT, PLAYER_FILEPATH), 1.0f);
	mMeshComp->PlayAnimation(game->GetAnimation(PLAYER_ANIMATION_SHOOT_WALK, PLAYER_FILEPATH), 1.0f);
	mMeshComp->PlayAnimation(game->GetAnimation(PLAYER_ANIMATION_WALK, PLAYER_FILEPATH), 1.0f);
	mMeshComp->PlayAnimation(game->GetAnimation(PLAYER_ANIMATION_RUN, PLAYER_FILEPATH), 1.0f);
	mMeshComp->PlayAnimation(game->GetAnimation(PLAYER_ANIMATION_IDLE, PLAYER_FILEPATH), 1.0f);
	
	// ���R���|�[�l���g�̐���
	mMoveComp = new PlayerMove(this);
	mMoveComp->SetIsFall(mIsGround);
	mCameraComp = new TPSCamera(this);
	
	// �X�e�[�g�̐ݒ�
	RegisterState(new PlayerIdle(this));
	RegisterState(new PlayerWalk(this));
	RegisterState(new PlayerRun(this));
	RegisterState(new PlayerShoot(this));
	RegisterState(new PlayerShootWalk(this));
	ChangeState(PLAYER_IDLE);

	// AABB�̐ݒ�
	AABB box = mMeshComp->GetMesh()->GetBox();
	box.SetMinMax(PLAYER_WIDTH, PLAYER_DEPTH, PLAYER_HEIGHT);
	mBoxComp->SetObjectBox(box);
	mBoxComp->SetShouldRotate(false);

	// �̗͂̐ݒ�
	mHealth = PLAYER_MAX_HEALTH;

	// �^�O�̐ݒ�
	mTag = CharacterTag::EPlayer;
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

void Player::Damage(float value)
{
	// �̗͂���_���[�W������
	mHealth -= value;
	mHealth = Math::Max(mHealth, 0.0f);
	mHealth = Math::Min(mHealth, PLAYER_MAX_HEALTH);

	// ���݂̗̑͂𐳋K������HUD�ɔ��f������
	GetGame()->GetHUD()->SetNormalizeHealth(mHealth / PLAYER_MAX_HEALTH);

	// �̗͂�0�ɂȂ����玀�S
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
	// �J�n�_���擾
	Vector3 start = GetPosition();
	Vector3 offset = PLAYER_SHOOT_POSITION_OFFSET;
	start += GetRight() * offset.x;
	start += GetForward() * offset.y;
	start.z += offset.z;
	
	// �{�[�����X�|�[������
	Bullet * bullet = new Bullet(GetGame(), CharacterTag::EEnemy);
	bullet->GetMeshComp()->SetMesh(GetGame()->GetRenderer()->GetMesh(PLAYERBULLET_FILEPATH));
	bullet->GetMoveComp()->SetMoveSpeed(Vector2(0, PLAYER_BULLET_SPEED));
	bullet->GetPointLightComp()->mDiffuseColor = Color::Green;
	bullet->GetPointLightComp()->mOuterRadius = 200.0f;
	bullet->GetPointLightComp()->mInnerRadius = 100.0f;
	bullet->SetPlayer(this);
	bullet->SetPosition(start);
	// �{�[������]�����ĐV��������������
	bullet->RotateToNewForward(GetForward());

	mAudioComp->PlayEvent(SE_SHOOT_P);
}