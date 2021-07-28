#pragma once
#include "Character.h"
#include "StateMachine.h"

#pragma region �v���g�^�C�v�錾
class Game;
class SkeletalMeshComponent;
class MoveComponent;
#pragma endregion

#define ENEMY_FILEPATH         "Assets/Models/Enemy01.fbx"     // �G���f���̃t�@�C���p�X
#define ENEMYBULLET_FILEPATH   "Assets/Models/EnemyBullet.fbx" // �G�̒e���f���̃t�@�C���p�X

#define ENEMY_ANIMATION_IDLE   "Enemy_Idle"         // �G��"�ҋ@"�A�j���[�V������
#define ENEMY_ANIMATION_MOVE   "Enemy_Move"         // �G��"�ړ�"�A�j���[�V������
#define ENEMY_ANIMATION_DEAD   "Enemy_Die"          // �G��"���S"�A�j���[�V������

#define ENEMY_SCALE 120.0f         // �X�P�[���l
#define ENEMY_WIDTH  1.2f          // AABB�̕� Vigilant
#define ENEMY_DEPTH  1.2f          // AABB�̉��s
#define ENEMY_HEIGHT 2.1f          // AABB�̍���
#define ENEMY_CENTER_OFFSET 130.0f // �G�̒��S�ʒu�ւ�Z�����̃I�t�Z�b�g�l

#define ENEMY_ATTACK_TIME           1.0f                      // �U������y�[�X
#define ENEMY_SHOOT_POSITION_OFFSET Vector3(0, 80.0f, 130.0f) // �e�̔��ˈʒu�̃I�t�Z�b�g�l
#define ENEMY_BULLET_SPEED          2000.0f                   // �e�̑��x

#define ENEMY_MAX_HEALTH       10.0f // �̗͂̍ő�l
#define ENEMY_DEADTIME          2.0f // ���S��ɏ��ł���܂ł̎��ԁi�b�j
#define ENEMY_RESTTIME          1.0f // �Î~���Ă��鎞�ԁi�b�j
#define ENEMY_ROTATE_SPEED      5.0f // ��]���x�̌W��
#define ENEMY_MAX_ROTATE_SPEED  2.0f // �ő��]���x
#define ENEMY_VISIBLE_RANGE  2000.0f // �����̓͂�����

class Enemy : public Character, public StateMachine
{
public:
	Enemy(Game* game);

	void UpdateActor(float deltaTime) override;

	MoveComponent* GetMoveComp() { return mMoveComp; }
	Vector3 GetCenterPosition() { return GetPosition() + Vector3(0, 0, ENEMY_CENTER_OFFSET); }

	void Damage(float value) override;
	void Dead() override;
	void Shoot();

	// �����̐�Ƀv���C���[�����邩�̔���
	bool CheckPlayerVisible();

private:

	MoveComponent* mMoveComp;
};