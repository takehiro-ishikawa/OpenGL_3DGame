#pragma once
#include "Character.h"
#include "StateMachine.h"
#include "SoundEvent.h"

#pragma region プロトタイプ宣言
class Game;
class PlayerMove;
class TPSCamera;
class SkeletalMeshComponent;
class BoxComponent;
class AudioComponent;
#pragma endregion

#define PLAYER_FILEPATH       "Assets/Player.fbx" // プレイヤーモデルのファイルパス
#define PLAYER_ANIMATION_IDLE "Idle"              // プレイヤーの"待機"アニメーション
#define PLAYER_ANIMATION_WALK "Walk"              // プレイヤーの"歩行"アニメーション

#define PLAYER_WIDTH  1.4f // AABBの幅
#define PLAYER_DEPTH  1.4f // AABBの奥行
#define PLAYER_HEIGHT 3.2f // AABBの高さ


class Player : public Character, public StateMachine
{
public:
	Player(Game* game);

	void ActorInput(const struct InputState& state) override;
	void UpdateActor(float deltaTime) override;

	void SetVisible(bool visible);
	PlayerMove* GetMoveComp() { return mMoveComp; }
	TPSCamera* GetCameraComp() { return mCameraComp; }
	bool GetIsGround() { return mIsGround; }

	// 弾の発射
	void Shoot();

private:

	PlayerMove* mMoveComp;
	TPSCamera* mCameraComp;

	SoundEvent mWalk;

	// 着地しているか
	bool mIsGround;
};