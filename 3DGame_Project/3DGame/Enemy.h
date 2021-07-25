#pragma once
#include "Character.h"
#include "StateMachine.h"

#pragma region �v���g�^�C�v�錾
class Game;
class SkeletalMeshComponent;
class MoveComponent;
#pragma endregion

#define ENEMY_FILEPATH         "Assets/Enemy01.fbx" // �G���f���̃t�@�C���p�X
#define ENEMY_ANIMATION_IDLE   "Enemy_Idle"         // �G��"�ҋ@"�A�j���[�V������
#define ENEMY_ANIMATION_MOVE   "Enemy_Move"         // �G��"�ړ�"�A�j���[�V������
#define ENEMY_ANIMATION_DEAD   "Enemy_Die"          // �G��"���S"�A�j���[�V������

#define ENEMY_SCALE 120.0f  // �X�P�[���l
#define ENEMY_WIDTH  1.2f   // AABB�̕� Vigilant
#define ENEMY_DEPTH  1.2f   // AABB�̉��s
#define ENEMY_HEIGHT 2.1f   // AABB�̍���

#define ENEMY_MAX_HEALTH      10.0f // �̗͂̍ő�l
#define ENEMY_DEADTIME         2.0f // ���S��ɏ��ł���܂ł̎��ԁi�b�j
#define ENEMY_RESTTIME         1.0f // �Î~���Ă��鎞�ԁi�b�j
#define ENEMY_ROTATE_SPEED     5.0f // ��]���x�̌W��
#define ENEMY_MAX_ROTATE_SPEED 2.0f // �ő��]���x

class Enemy : public Character, public StateMachine
{
public:
	Enemy(Game* game);

	void UpdateActor(float deltaTime) override;

	MoveComponent* GetMoveComp() { return mMoveComp; }

	void Damage(float value) override;
	void Dead() override;

private:
	MoveComponent* mMoveComp;
};