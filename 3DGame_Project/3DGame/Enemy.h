#pragma once
#include "Character.h"
#include "StateMachine.h"

#pragma region プロトタイプ宣言
class Game;
class SkeletalMeshComponent;
#pragma endregion

#define ENEMY_FILEPATH           "Assets/Enemy01.fbx" // 敵モデルのファイルパス
#define ENEMY_ANIMATION_VIGILANT "Vigilant"           // 敵の"警戒"アニメーション名
#define ENEMY_ANIMATION_DEAD     "Die"                // 敵の"死亡"アニメーション名

#define ENEMY_WIDTH  1.2f   // AABBの幅 Vigilant
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