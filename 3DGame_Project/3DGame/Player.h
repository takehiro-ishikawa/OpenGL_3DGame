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


class Player : public Character, public StateMachine
{
public:
	Player(Game* game);

	void ActorInput(const struct InputState& state) override;
	void UpdateActor(float deltaTime) override;
	void Damage(float value) override;
	void Dead() override;

	void SetVisible(bool visible);
	PlayerMove* GetMoveComp() { return mMoveComp; }
	TPSCamera* GetCameraComp() { return mCameraComp; }
	bool GetIsGround() { return mIsGround; }

	// ジャンプ
	void Jump();

	// 弾の発射
	void Shoot();

private:

	PlayerMove* mMoveComp;
	TPSCamera* mCameraComp;

	SoundEvent mWalk;

	// 着地しているか
	bool mIsGround;
};