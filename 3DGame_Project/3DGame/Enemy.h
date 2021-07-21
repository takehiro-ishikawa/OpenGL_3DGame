#pragma once
#include "Character.h"
#include "StateMachine.h"

#pragma region �v���g�^�C�v�錾
class Game;
class SkeletalMeshComponent;
#pragma endregion

#define ENEMY_FILEPATH           "Assets/Enemy01.fbx" // �G���f���̃t�@�C���p�X
#define ENEMY_ANIMATION_VIGILANT "Vigilant"           // �G��"�x��"�A�j���[�V������
#define ENEMY_ANIMATION_DEAD     "Die"                // �G��"���S"�A�j���[�V������

#define ENEMY_WIDTH  1.2f   // AABB�̕� Vigilant
#define ENEMY_DEPTH  1.2f   // AABB�̉��s
#define ENEMY_HEIGHT 2.1f   // AABB�̍���

#define ENEMY_DEADTIME 2.0f // ���S��ɏ��ł���܂ł̎��ԁi�b�j

class Enemy : public Character, public StateMachine
{
public:
	Enemy(Game* game);

	void UpdateActor(float deltaTime) override;

	void Damage() override;

private:
	
};