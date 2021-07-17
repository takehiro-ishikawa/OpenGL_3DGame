#pragma once
#include "Character.h"
#include "SoundEvent.h"

#pragma region �v���g�^�C�v�錾
class Game;
class PlayerMove;
class TPSCamera;
class SkeletalMeshComponent;
class BoxComponent;
class AudioComponent;
#pragma endregion

#define PLAYER_WIDTH  1.4f // AABB�̕�
#define PLAYER_DEPTH  1.4f // AABB�̉��s
#define PLAYER_HEIGHT 3.2f // AABB�̍���

// �v���C���[�̏��
enum class PlayerState
{
	Idle, // �ҋ@
	Walk  // ���s
};

class Player : public Character
{
public:
	Player(Game* game);

	void ActorInput(const struct InputState& state) override;
	void UpdateActor(float deltaTime) override;

	void SetVisible(bool visible);

private:

	// ���n����
	bool CheckGround();

	// ��ԑJ��
	void ChangeIdle();
	void ChangeWalk();

	PlayerMove* mMoveComp;
	TPSCamera* mCameraComp;
	SkeletalMeshComponent* mMeshComp;

	SoundEvent mWalk;

	// ���݂̏��
	PlayerState mCurrentState;

	// ���n���Ă��邩
	bool mIsGround;
};