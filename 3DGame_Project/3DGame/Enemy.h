#pragma once
#include "Character.h"

#pragma region プロトタイプ宣言
class Game;
class SkeletalMeshComponent;
class BoxComponent;
class AudioComponent;
#pragma endregion

#define ENEMY_WIDTH  1.2f // AABBの幅
#define ENEMY_DEPTH  1.2f // AABBの奥行
#define ENEMY_HEIGHT 2.1f // AABBの高さ

class Enemy : public Character
{
public:
	Enemy(Game* game);

private:
	SkeletalMeshComponent* mMeshComp;
};