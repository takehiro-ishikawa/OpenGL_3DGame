#pragma once
#include "Character.h"
#include "StateMachine.h"
#include "Audio.h"

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

#define ENEMY_SCALE 120.0f                          // �X�P�[���l
#define ENEMY_WIDTH  1.2f                           // AABB�̕� Vigilant
#define ENEMY_DEPTH  1.2f                           // AABB�̉��s
#define ENEMY_HEIGHT 2.1f                           // AABB�̍���
#define ENEMY_CENTER_OFFSET Vector3(0, 0, 150.0f)   // �G�̒��S�ʒu�ւ̃I�t�Z�b�g�l

#define ENEMY_ATTACK_TIME           1.0f                      // �U������y�[�X
#define ENEMY_SHOOT_POSITION_OFFSET Vector3(0, 80.0f, 150.0f) // �e�̔��ˈʒu�̃I�t�Z�b�g�l
#define ENEMY_BULLET_DAMAGE            2.0f                   // �e�̈З�
#define ENEMY_BULLET_SPEED          2000.0f                   // �e�̑��x
#define ENEMY_BULLET_LIGHT_OUTER     200.0f                   // �e�̓_�����̊O���͈̔�
#define ENEMY_BULLET_LIGHT_INNER     100.0f                   // �e�̓_�����̓����͈̔�

#define ENEMY_MAX_HEALTH        5.0f // �̗͂̍ő�l
#define ENEMY_DEADTIME          1.0f // ���S��ɏ��ł���܂ł̎��ԁi�b�j
#define ENEMY_MOVESPEED       300.0f // �ړ����x(�b)
#define ENEMY_RESTTIME          1.0f // �Î~���Ă��鎞��
#define ENEMY_ROTATE_SPEED     10.0f // ��]���x�̌W��
#define ENEMY_MAX_ROTATE_SPEED  2.0f // �ő��]���x
#define ENEMY_VISIBLE_ANGLE    45.0f // ����p
#define ENEMY_VISIBLE_RANGE  2500.0f // �����̓͂�����
#define ENEMY_ATTACK_RANGE   1000.0f // �U�����J�n���鋗��

class Enemy : public Character, public StateMachine
{
public:
	Enemy(Game* game);

	void UpdateActor(float deltaTime) override;

	MoveComponent* GetMoveComp() { return mMoveComp; }

	void Damage(float value) override;
	void Dead() override;
	void Shoot();
	void PlayFootStep(bool isPlay);

	// ���E���Ƀv���C���[�����邩�̔���
	bool CheckInViewPlayer(float angle, float length);

	// ���g�ƃv���C���[�̊Ԃ��Ղ镨���Ȃ����̔���
	bool CheckInterruptObject();

private:

	MoveComponent* mMoveComp;
	SoundEvent mFootStep;
};