#pragma once
#include "Character.h"
#include "StateMachine.h"

#pragma region プロトタイプ宣言
class Game;
class SkeletalMeshComponent;
class MoveComponent;
#pragma endregion

#define ENEMY_FILEPATH         "Assets/Enemy01.fbx" // 敵モデルのファイルパス
#define ENEMY_ANIMATION_IDLE   "Enemy_Idle"         // 敵の"待機"アニメーション名
#define ENEMY_ANIMATION_MOVE   "Enemy_Move"         // 敵の"移動"アニメーション名
#define ENEMY_ANIMATION_DEAD   "Enemy_Die"          // 敵の"死亡"アニメーション名

#define ENEMY_SCALE 120.0f  // スケール値
#define ENEMY_WIDTH  1.2f   // AABBの幅 Vigilant
#define ENEMY_DEPTH  1.2f   // AABBの奥行
#define ENEMY_HEIGHT 2.1f   // AABBの高さ

#define ENEMY_MAX_HEALTH      10.0f // 体力の最大値
#define ENEMY_DEADTIME         2.0f // 死亡後に消滅するまでの時間（秒）
#define ENEMY_RESTTIME         1.0f // 静止している時間（秒）
#define ENEMY_ROTATE_SPEED     5.0f // 回転速度の係数
#define ENEMY_MAX_ROTATE_SPEED 2.0f // 最大回転速度

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