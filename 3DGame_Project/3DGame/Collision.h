#pragma once
#include "Math.h"
#include <vector>

// 線分
struct LineSegment
{
	LineSegment(const Vector3& start, const Vector3& end);

	// 0 <= t <= 1であるセグメントに沿ってポイントを取得
	Vector3 PointOnSegment(float t) const;

	// 座標と線分の間の最短距離の2乗を取得
	float MinDistSq(const Vector3& point) const;

	// 線分と線分との間の最短距離を計算する
	static float MinDistSq(const LineSegment& s1, const LineSegment& s2);

	Vector3 mStart; // 始点
	Vector3 mEnd;   // 終点
};

// 平面
struct Plane
{
	Plane(const Vector3& normal, float d);

	// 3点から平面を構築する
	Plane(const Vector3& a, const Vector3& b, const Vector3& c);

	// 点と平面の間の符号付き距離を取得
	float SignedDist(const Vector3& point) const;

	Vector3 mNormal; // 法線
	float mD;        // 平面と原点との符号付き(最短)距離
};

// 球
struct Sphere
{
	Sphere(const Vector3& center, float radius);
	bool Contains(const Vector3& point) const;

	Vector3 mCenter; // 中心位置
	float mRadius;   // 半径
};

// 軸平行バウンディングボックス（どの辺もX軸かY軸に平行な箱）
struct AABB
{
	AABB(const Vector3& min, const Vector3& max);

	// 受け取った点の値に応じて最小点minと最大点maxを更新する（モデルのロード時に使用される）
	void UpdateMinMax(const Vector3& point);

	// 指定した幅、高さ、奥行で最小点minと最大点maxを設定する
	void SetMinMax(float width, float depth, float height);

	// オブジェクトの回転に合わせてAABBの寸法を変更する
	void Rotate(const Quaternion& q);

	// AABBの包含判定
	bool Contains(const Vector3& point) const;

	// 点とAABBとの間の最短距離を計算する
	float MinDistSq(const Vector3& point) const;

	Vector3 mMin; // 最小の点
	Vector3 mMax; // 最大の点
};

// 有向バウンディングボックス（軸平行の制限を持たない箱）
struct OBB
{
	Vector3 mCenter;      // 中心の位置
	Quaternion mRotation; // 回転クォータニオン
	Vector3 mExtents;     // ボックスの広がり
};

// カプセル
struct Capsule
{
	Capsule(const Vector3& start, const Vector3& end, float radius);

	// 0 <= t <= 1であるセグメントに沿ってポイントを取得
	Vector3 PointOnSegment(float t) const;

	bool Contains(const Vector3& point) const;

	LineSegment mSegment; // 線分
	float mRadius;        // 半径
};

// 凸ポリゴン
struct ConvexPolygon
{
	bool Contains(const Vector2& point) const;

	// 頂点には時計回りの順序がある
	std::vector<Vector2> mVertices;
};

// 交差判定の関数
bool Intersect(const Sphere& a, const Sphere& b);
bool Intersect(const AABB& a, const AABB& b);
bool Intersect(const Capsule& a, const Capsule& b);
bool Intersect(const Sphere& s, const AABB& box);

bool Intersect(const LineSegment& l, const Sphere& s, float& outT);
bool Intersect(const LineSegment& l, const Plane& p, float& outT);
bool Intersect(const LineSegment& l, const AABB& b, float& outT,
	Vector3& outNorm);

bool SweptSphere(const Sphere& P0, const Sphere& P1,
	const Sphere& Q0, const Sphere& Q1, float& t);
