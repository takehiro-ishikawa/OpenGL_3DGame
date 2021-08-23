#pragma once
#include "Component.h"
#include "PhysWorld.h"

// プロトタイプ宣言
class Actor;

// 当たり判定を行うボックスのコンポーネント
class BoxComponent : public Component
{
public:
	BoxComponent(Actor* owner, int updateOrder = 100);
	~BoxComponent();

	void OnUpdateWorldTransform() override;

	// ゲッター/セッター
	void SetObjectBox(const AABB& model) { mObjectBox = model; }
	const AABB& GetWorldBox() const { return mWorldBox; }
	void SetShouldRotate(bool value) { mShouldRotate = value; }

private:
	AABB mObjectBox;    // オブジェクト空間のAABB
	AABB mWorldBox;     // ワールド空間のAABB
	bool mShouldRotate; // 所有アクターの回転に合わせて回転させるか
};