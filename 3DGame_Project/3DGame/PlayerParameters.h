#pragma once
#include "Math.h"

// プレイヤーに関係する各種パラメーターを設定する

#define PLAYER_FILEPATH             "Assets/Player.fbx" // プレイヤーモデルのファイルパス
#define PLAYER_ANIMATION_IDLE       "Player_Idle"              // プレイヤーの"待機"アニメーション名
#define PLAYER_ANIMATION_WALK       "Player_Walk"              // プレイヤーの"歩行"アニメーション名
#define PLAYER_ANIMATION_RUN        "Player_Run"               // プレイヤーの"ダッシュ"アニメーション名
#define PLAYER_ANIMATION_SHOOT      "Player_Shoot"             // プレイヤーの"射撃"アニメーション名
#define PLAYER_ANIMATION_SHOOT_WALK "Player_ShootAndWalk"      // プレイヤーの"射撃&歩行"アニメーション名

#define PLAYER_WIDTH  1.4f // AABBの幅
#define PLAYER_DEPTH  1.4f // AABBの奥行
#define PLAYER_HEIGHT 3.2f // AABBの高さ

#define PLAYER_WALK_SPEED   400.0f // 歩行速度
#define PLAYER_RUN_SPEED    800.0f // ダッシュ速度
#define PLAYER_ROTATE_SPEED 10.0f  // 回転速度
#define PLAYER_JUMP_SPEED   800.0f // ジャンプ初速度

#define PLAYER_MAX_HEALTH 100.0f // 体力の最大値

#define PLAYER_SHOOT_POSITION_OFFSET Vector3(50.0f, 60.0f, 130.0f);

#define CAMERA_NORMAL_DIST       600.0f // 通常時の所有アクターとの距離
#define CAMERA_AIM_DIST          400.0f // 射撃時の所有アクターとの距離
#define CAMERA_NORMAL_YAWSPEED   Math::Pi * 25
#define CAMERA_NORMAL_PITCHSPEED Math::Pi * 8
#define CAMERA_AIM_YAWSPEED      Math::Pi * 10
#define CAMERA_AIM_PITCHSPEED    Math::Pi * 5
#define CAMERA_NORMAL_OFFSETPOS  Vector3(  0,    0, 300.0f) // 通常時のカメラ位置のオフセット値
#define CAMERA_AIM_OFFSETPOS     Vector3(100.0f, 0, 250.0f)