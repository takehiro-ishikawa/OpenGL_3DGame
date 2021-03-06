#pragma once
#include "Math.h"

// プレイヤーに関係する各種パラメーターを設定する

#define PLAYER_FILEPATH             "Assets/Models/Player.fbx"       // プレイヤーモデルのファイルパス
#define PLAYERBULLET_FILEPATH       "Assets/Models/PlayerBullet.fbx" // プレイヤーの弾モデルのファイルパス
#define PLAYER_ANIMATION_IDLE       "Player_Idle"                    // プレイヤーの"待機"アニメーション名
#define PLAYER_ANIMATION_WALK       "Player_Walk"                    // プレイヤーの"歩行"アニメーション名
#define PLAYER_ANIMATION_RUN        "Player_Run"                     // プレイヤーの"ダッシュ"アニメーション名
#define PLAYER_ANIMATION_SHOOT      "Player_Shoot"                   // プレイヤーの"射撃"アニメーション名
#define PLAYER_ANIMATION_SHOOT_WALK "Player_ShootAndWalk"            // プレイヤーの"射撃&歩行"アニメーション名
#define PLAYER_ANIMATION_ATTACK     "Player_Attack"                  // プレイヤーの"近接攻撃"アニメーション名
#define PLAYER_ANIMATION_DEAD       "Player_Dead"                    // プレイヤーの"死亡"アニメーション名

#define PLAYER_WIDTH  1.4f                         // AABBの幅
#define PLAYER_DEPTH  1.4f                         // AABBの奥行
#define PLAYER_HEIGHT 3.2f                         // AABBの高さ
#define PLAYER_CENTER_OFFSET Vector3(0, 0, 10.0f) // プレイヤーの中心位置へのオフセット値

#define PLAYER_WALK_SPEED   400.0f // 歩行速度
#define PLAYER_RUN_SPEED    800.0f // ダッシュ速度
#define PLAYER_ROTATE_SPEED 10.0f  // 回転速度
#define PLAYER_JUMP_SPEED   800.0f // ジャンプ初速度

#define PLAYER_MAX_HEALTH 100.0f // 体力の最大値
#define PLAYER_DEADTIME     1.5f // 死亡後にリザルト画面を出す時間

#define PLAYER_ATTACK_RANGE  300.0f  // 近接攻撃の届く範囲
#define PLAYER_ATTACK_TIME     0.15f // 攻撃用関数が呼び出されるタイミング(秒)
#define PLAYER_ATTACK_END_TIME 0.5f  // 近接攻撃が終了する時間(秒)

#define PLAYER_SHOOT_POSITION_OFFSET Vector3(50.0f, 60.0f, 130.0f) // 弾の発射位置のオフセット値
#define PLAYER_BULLET_SPEED      5000.0f // 弾の速度
#define PLAYER_BULLET_DAMAGE        1.0f // 弾の威力
#define PLAYER_BULLET_LIGHT_OUTER 200.0f // 弾の点光源の外側の範囲
#define PLAYER_BULLET_LIGHT_INNER 100.0f // 弾の点光源の内側の範囲

#define CAMERA_NORMAL_DIST       600.0f // 通常時の所有アクターとの距離
#define CAMERA_AIM_DIST          400.0f // 射撃時の所有アクターとの距離
#define CAMERA_NORMAL_YAWSPEED   Math::Pi * 25
#define CAMERA_NORMAL_PITCHSPEED Math::Pi * 8
#define CAMERA_AIM_YAWSPEED      Math::Pi * 10
#define CAMERA_AIM_PITCHSPEED    Math::Pi * 5
#define CAMERA_NORMAL_OFFSETPOS  Vector3(  0,    0, 300.0f) // 通常時のカメラ位置のオフセット値
#define CAMERA_AIM_OFFSETPOS     Vector3(100.0f, 0, 250.0f)