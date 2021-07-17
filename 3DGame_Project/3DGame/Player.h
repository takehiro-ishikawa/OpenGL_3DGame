#pragma once
#include "Character.h"
#include "SoundEvent.h"

#pragma region プロトタイプ宣言
class Game;
class PlayerMove;
class TPSCamera;
class SkeletalMeshComponent;
class BoxComponent;
class AudioComponent;
#pragma endregion

#define PLAYER_WIDTH  1.4f // AABBの幅
#define PLAYER_DEPTH  1.4f // AABBの奥行
#define PLAYER_HEIGHT 3.2f // AABBの高さ

// プレイヤーの状態
enum class PlayerState
{
	Idle, // 待機
	Walk  // 歩行
};

class Player : public Character
{
public:
	Player(Game* game);

	void ActorInput(const struct InputState& state) override;
	void UpdateActor(float deltaTime) override;

	void SetVisible(bool visible);

private:

	// 着地判定
	bool CheckGround();

	// 状態遷移
	void ChangeIdle();
	void ChangeWalk();

	PlayerMove* mMoveComp;
	TPSCamera* mCameraComp;
	SkeletalMeshComponent* mMeshComp;

	SoundEvent mWalk;

	// 現在の状態
	PlayerState mCurrentState;

	// 着地しているか
	bool mIsGround;
};