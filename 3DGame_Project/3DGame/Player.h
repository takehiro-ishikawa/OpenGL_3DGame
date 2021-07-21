#pragma once
#include "Character.h"
#include "StateMachine.h"
#include "SoundEvent.h"

#pragma region �v���g�^�C�v�錾
class Game;
class PlayerMove;
class TPSCamera;
class SkeletalMeshComponent;
class BoxComponent;
class AudioComponent;
#pragma endregion

#define PLAYER_FILEPATH       "Assets/Player.fbx" // �v���C���[���f���̃t�@�C���p�X
#define PLAYER_ANIMATION_IDLE "Idle"              // �v���C���[��"�ҋ@"�A�j���[�V����
#define PLAYER_ANIMATION_WALK "Walk"              // �v���C���[��"���s"�A�j���[�V����

#define PLAYER_WIDTH  1.4f // AABB�̕�
#define PLAYER_DEPTH  1.4f // AABB�̉��s
#define PLAYER_HEIGHT 3.2f // AABB�̍���


class Player : public Character, public StateMachine
{
public:
	Player(Game* game);

	void ActorInput(const struct InputState& state) override;
	void UpdateActor(float deltaTime) override;

	void SetVisible(bool visible);
	PlayerMove* GetMoveComp() { return mMoveComp; }
	TPSCamera* GetCameraComp() { return mCameraComp; }
	bool GetIsGround() { return mIsGround; }

	// �e�̔���
	void Shoot();

private:

	PlayerMove* mMoveComp;
	TPSCamera* mCameraComp;

	SoundEvent mWalk;

	// ���n���Ă��邩
	bool mIsGround;
};