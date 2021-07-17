#include "PhysWorld.h"
#include <algorithm>
#include "BoxComponent.h"
#include <SDL.h>

PhysWorld::PhysWorld(Game* game)
	:mGame(game)
{
}

bool PhysWorld::SegmentCast(const LineSegment& l, CollisionInfo& outColl)
{
	bool collided = false;

	// closestTは無限大で初期化する
	// （最初の交差で必ず更新される）
	float closestT = Math::Infinity;
	Vector3 norm;

	// 全てのボックスをテストする
	for (auto box : mBoxes)
	{
		float t;

		// 線分はボックスと交差するか?
		if (Intersect(l, box->GetWorldBox(), t, norm))
		{
			// 前の交差よりも近いか?
			if (t < closestT)
			{
				closestT = t;
				outColl.mPoint = l.PointOnSegment(t);
				outColl.mNormal = norm;
				outColl.mBox = box;
				outColl.mActor = box->GetOwner();
				collided = true;
			}
		}
	}
	return collided;
}

bool PhysWorld::SegmentCast(const LineSegment& l, CollisionInfo& outColl, Actor* myActor)
{
	bool collided = false;

	// closestTは無限大で初期化する
	// （最初の交差で必ず更新される）
	float closestT = Math::Infinity;
	Vector3 norm;

	// 全てのボックスをテストする
	for (auto box : mBoxes)
	{
		float t;

		// 線分はボックスと交差するか?
		if (box->GetOwner() != myActor && Intersect(l, box->GetWorldBox(), t, norm))
		{
			// 前の交差よりも近いか?
			if (t < closestT)
			{
				closestT = t;
				outColl.mPoint = l.PointOnSegment(t);
				outColl.mNormal = norm;
				outColl.mBox = box;
				outColl.mActor = box->GetOwner();
				collided = true;
			}
		}
	}
	return collided;
}

void PhysWorld::TestPairwise(std::function<void(Actor*, Actor*)> f)
{
	// 素直な実装(n^2)
	for (size_t i = 0; i < mBoxes.size(); i++)
	{
		// 自分自身と既にテストしたiの値とは、テストしない
		for (size_t j = i + 1; j < mBoxes.size(); j++)
		{
			BoxComponent* a = mBoxes[i];
			BoxComponent* b = mBoxes[j];
			if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
			{
				// 提供された関数を呼び出して交差を判定させる
				f(a->GetOwner(), b->GetOwner());
			}
		}
	}
}

void PhysWorld::TestSweepAndPrune(std::function<void(Actor*, Actor*)> f)
{
	// min.xの最小値の昇順でソートする
	std::sort(mBoxes.begin(), mBoxes.end(),
		[](BoxComponent* a, BoxComponent* b) {
		return a->GetWorldBox().mMin.x < b->GetWorldBox().mMin.x;
	});

	for (size_t i = 0; i < mBoxes.size(); i++)
	{
		// box[i]のmax.xを取得
		BoxComponent* a = mBoxes[i];
		float max = a->GetWorldBox().mMax.x;
		for (size_t j = i + 1; j < mBoxes.size(); j++)
		{
			BoxComponent* b = mBoxes[j];

			// もしbox[j]のmin.xが、box[i]のmax.x境界を超えていたら
			// box[i]と交差する可能性があるボックスは、他に存在しない
			if (b->GetWorldBox().mMin.x > max)
			{
				break;
			}
			else if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
			{
				f(a->GetOwner(), b->GetOwner());
			}
		}
	}
}

void PhysWorld::AddBox(BoxComponent* box)
{
	mBoxes.emplace_back(box);
}

void PhysWorld::RemoveBox(BoxComponent* box)
{
	auto iter = std::find(mBoxes.begin(), mBoxes.end(), box);
	if (iter != mBoxes.end())
	{
		// ベクトルの最後にスワップしてポップオフする（コピーの消去は避けること）
		std::iter_swap(iter, mBoxes.end() - 1);
		mBoxes.pop_back();
	}
}
