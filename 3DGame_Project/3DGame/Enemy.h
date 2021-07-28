#pragma once
#include "Character.h"
#include "StateMachine.h"

#pragma region プロトタイプ宣言
class Game;
class SkeletalMeshComponent;
class MoveComponent;
#pragma endregion

#define ENEMY_FILEPATH         "Assets/Models/Enemy01.fbx"     // 敵モデルのファイルパス
#define ENEMYBULLET_FILEPATH   "Assets/Models/EnemyBullet.fbx" // 敵の弾モデルのファイルパス

#define ENEMY_ANIMATION_IDLE   "Enemy_Idle"         // 敵の"待機"アニメーション名
#define ENEMY_ANIMATION_MOVE   "Enemy_Move"         // 敵の"移動"アニメーション名
#define ENEMY_ANIMATION_DEAD   "Enemy_Die"          // 敵の"死亡"アニメーション名

#define ENEMY_SCALE 120.0f         // スケール値
#define ENEMY_WIDTH  1.2f          // AABBの幅 Vigilant
#define ENEMY_DEPTH  1.2f          // AABBの奥行
#define ENEMY_HEIGHT 2.1f          // AABBの高さ
#define ENEMY_CENTER_OFFSET 130.0f // 敵の中心位置へのZ方向のオフセット値

#define ENEMY_ATTACK_TIME           1.0f                      // 攻撃するペース
#define ENEMY_SHOOT_POSITION_OFFSET Vector3(0, 80.0f, 130.0f) // 弾の発射位置のオフセット値
#define ENEMY_BULLET_SPEED          2000.0f                   // 弾の速度

#define ENEMY_MAX_HEALTH       10.0f // 体力の最大値
#define ENEMY_DEADTIME          2.0f // 死亡後に消滅するまでの時間（秒）
#define ENEMY_RESTTIME          1.0f // 静止している時間（秒）
#define ENEMY_ROTATE_SPEED      5.0f // 回転速度の係数
#define ENEMY_MAX_ROTATE_SPEED  2.0f // 最大回転速度
#define ENEMY_VISIBLE_RANGE  2000.0f // 視線の届く距離

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

	// 視線の先にプレイヤーがいるかの判定
	bool CheckPlayerVisible();

private:

	MoveComponent* mMoveComp;
};