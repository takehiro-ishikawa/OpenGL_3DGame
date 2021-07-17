#pragma once
#include <cstdint>

// プロトタイプ宣言
class Actor;

class Component
{
public:
	// コンストラクタ
	//（更新順序が低いほど、コンポーネントの更新が早くなる）
	Component(Actor* owner, int updateOrder = 100);

	// デストラクタ
	virtual ~Component();

	// このコンポーネントをデルタタイムで更新する
	virtual void Update(float deltaTime);

	// このコンポーネントのプロセス入力
	virtual void ProcessInput(const struct InputState& state) {}

	// ワールド座標が変化した時に呼び出される
	virtual void OnUpdateWorldTransform() { }

	Actor* GetOwner() { return mOwner; }
	int GetUpdateOrder() const { return mUpdateOrder; }

protected:
	// 所有アクター
	Actor* mOwner;
	// コンポーネントの順序を更新する
	int mUpdateOrder;
};

