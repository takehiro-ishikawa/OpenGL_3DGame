#pragma once
#include "Character.h"
#include "StateMachine.h"

#pragma region プロトタイプ宣言
class Game;
class SkeletalMeshComponent;
#pragma endregion

#define ENEMY_WIDTH  1.2f   // AABBの幅
#define ENEMY_DEPTH  1.2f   // AABBの奥行
#define ENEMY_HEIGHT 2.1f   // AABBの高さ

#define ENEMY_DEADTIME 2.0f // 死亡後に消滅するまでの時間（秒）

class Enemy : public Character, public StateMachine
{
public:
	Enemy(Game* game);

	void UpdateActor(float deltaTime) override;

	void Damage() override;

private:
	
};