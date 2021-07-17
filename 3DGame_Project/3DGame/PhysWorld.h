#pragma once
#include <vector>
#include <functional>
#include "Collision.h"

#pragma region プロトタイプ宣言
class Game;
class BoxComponent;
class Actor;
#pragma endregion

// 全ての当たり判定を管理
class PhysWorld
{
public:
	PhysWorld(Game* game);

	// 衝突結果に関する有用な情報を提供するために使用する
	struct CollisionInfo
	{
		// 衝突した位置
		Vector3 mPoint;

		// 衝突点の法線
		Vector3 mNormal;

		// 交差したBoxComponentへのポインタ
		BoxComponent* mBox;

		// 交差したBoxComponentの所有アクターへのポインタ
		Actor* mActor;
	};

	// 受け取った線分と全てのボックスとの交差判定を行う
	// ボックスに衝突した場合はtrueを返す
	bool SegmentCast(const LineSegment& l, CollisionInfo& outColl);
	bool SegmentCast(const LineSegment& l, CollisionInfo& outColl, Actor* myActor);

	// ワールドにある全てのボックスに関して衝突をテストする
	void TestPairwise(std::function<void(Actor*, Actor*)> f);
	// 特定の軸に沿って区間が重なるボックスだけ衝突をテストする
	// 「二つのボックスは、1つの座標軸で区間が重なり合わなければ交差しない」という事実を利用する
	void TestSweepAndPrune(std::function<void(Actor*, Actor*)> f);

	// ワールドにボックスコンポーネントを追加/削除
	void AddBox(BoxComponent* box);
	void RemoveBox(BoxComponent* box);

	const std::vector<BoxComponent*>& GetBoxes() { return mBoxes; }

private:
	Game* mGame;
	std::vector<BoxComponent*> mBoxes;
};