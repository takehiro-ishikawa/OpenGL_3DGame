#pragma once
#include <vector>
#include "Math.h"
#include <cstdint>

class Actor
{
public:
	enum class ActorState
	{
		EActive,
		EPaused,
		EDead
	};

	Actor(class Game* game);
	virtual ~Actor();

	// Gameから呼び出されるUpdate関数（オーバーライド不可）
	void Update(float deltaTime);
	// アクターに持つ全てのコンポーネントを更新する（オーバーライド不可）
	void UpdateComponents(float deltaTime);
	// アクター固有の更新コード（オーバーライド可能）
	virtual void UpdateActor(float deltaTime);

	// ゲームから呼び出されるProcessInput関数（オーバーライド不可）
	void ProcessInput(const struct InputState& state);
	// アクター固有の入力コード（オーバーライド可能）
	virtual void ActorInput(const struct InputState& state);

	// ワールド行列
	void ComputeWorldTransform();                                        // ワールド行列を計算
	const Matrix4& GetWorldTransform() const { return mWorldTransform; } // ワールド行列を取得

	// ゲッター/セッター
	const Vector3& GetPosition() const { return mPosition; }
	void SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeWorldTransform = true; }

	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale;  mRecomputeWorldTransform = true; }

	const Quaternion& GetRotation() const { return mRotation; }
	void SetRotation(const Quaternion& rotation) { mRotation = rotation;  mRecomputeWorldTransform = true; }
	
	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); } // 前方ベクトル
	Vector3 GetRight() const { return Vector3::Transform(Vector3::UnitY, mRotation); }   // 右方ベクトル
	Vector3 GetUpward() const { return Vector3::Transform(Vector3::UnitZ, mRotation); }  // 上方ベクトル

	ActorState GetState() const { return mState; }      // アクターの状態
	void SetState(ActorState state) { mState = state; } //     〃

	class Game* GetGame() { return mGame; }        // Gameへのポインタ

	// アクターの進行方向を新しい方向へと回転させる
	void RotateToNewForward(const Vector3& forward);

	// コンポーネントの追加/削除
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	// アクターの状態
	ActorState mState;

	// Transform
	Matrix4 mWorldTransform;       // ワールド座標
	Vector3 mPosition;             // 位置
	Quaternion mRotation;          // 回転
	float mScale;                  // スケール
	bool mRecomputeWorldTransform; // ワールド行列を再計算する必要があるか

	std::vector<class Component*> mComponents; // 自身のコンポーネント達
	class Game* mGame;                         // Gameへのポインタ
};