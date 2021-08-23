#pragma once
#include "Character.h"
#include "StateMachine.h"
#include "Audio.h"

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

#define ENEMY_SCALE 120.0f                          // スケール値
#define ENEMY_WIDTH  1.2f                           // AABBの幅 Vigilant
#define ENEMY_DEPTH  1.2f                           // AABBの奥行
#define ENEMY_HEIGHT 2.1f                           // AABBの高さ
#define ENEMY_CENTER_OFFSET Vector3(0, 0, 150.0f)   // 敵の中心位置へのオフセット値

#define ENEMY_ATTACK_TIME           1.0f                      // 攻撃するペース
#define ENEMY_SHOOT_POSITION_OFFSET Vector3(0, 80.0f, 150.0f) // 弾の発射位置のオフセット値
#define ENEMY_BULLET_DAMAGE            2.0f                   // 弾の威力
#define ENEMY_BULLET_SPEED          2000.0f                   // 弾の速度
#define ENEMY_BULLET_LIGHT_OUTER     200.0f                   // 弾の点光源の外側の範囲
#define ENEMY_BULLET_LIGHT_INNER     100.0f                   // 弾の点光源の内側の範囲

#define ENEMY_MAX_HEALTH        5.0f // 体力の最大値
#define ENEMY_DEADTIME          1.0f // 死亡後に消滅するまでの時間（秒）
#define ENEMY_MOVESPEED       300.0f // 移動速度(秒)
#define ENEMY_RESTTIME          1.0f // 静止している時間
#define ENEMY_ROTATE_SPEED     10.0f // 回転速度の係数
#define ENEMY_MAX_ROTATE_SPEED  2.0f // 最大回転速度
#define ENEMY_VISIBLE_ANGLE    45.0f // 視野角
#define ENEMY_VISIBLE_RANGE  2500.0f // 視線の届く距離
#define ENEMY_ATTACK_RANGE   1000.0f // 攻撃を開始する距離

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

	// 視界内にプレイヤーがいるかの判定
	bool CheckInViewPlayer(float angle, float length);

	// 自身とプレイヤーの間を遮る物がないかの判定
	bool CheckInterruptObject();

private:

	MoveComponent* mMoveComp;
	SoundEvent mFootStep;
};