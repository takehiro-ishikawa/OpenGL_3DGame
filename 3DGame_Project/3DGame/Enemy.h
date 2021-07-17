#pragma once
#include "Character.h"

#pragma region �v���g�^�C�v�錾
class Game;
class SkeletalMeshComponent;
class BoxComponent;
class AudioComponent;
#pragma endregion

#define ENEMY_WIDTH  1.2f // AABB�̕�
#define ENEMY_DEPTH  1.2f // AABB�̉��s
#define ENEMY_HEIGHT 2.1f // AABB�̍���

class Enemy : public Character
{
public:
	Enemy(Game* game);

private:
	SkeletalMeshComponent* mMeshComp;
};