#include "PhysWorld.h"
#include <algorithm>
#include <array>

LineSegment::LineSegment(const Vector3& start, const Vector3& end)
	:mStart(start)
	, mEnd(end)
{
}

// tを入力として線分上の点を返す
Vector3 LineSegment::PointOnSegment(float t) const
{
	return mStart + (mEnd - mStart) * t;
}

// 点と線分との間の最短距離を計算する
float LineSegment::MinDistSq(const Vector3& point) const
{
	// ベクトルを準備
	Vector3 ab = mEnd - mStart;
	Vector3 ba = -1.0f * ab;
	Vector3 ac = point - mStart;
	Vector3 bc = point - mEnd;

	// Case 1: C が A の前に突き出ている
	if (Vector3::Dot(ab, ac) < 0.0f)
	{
		return ac.LengthSq();
	}
	// Case 2: C が B のあとに突き出ている
	else if (Vector3::Dot(ba, bc) < 0.0f)
	{
		return bc.LengthSq();
	}
	// Case 3: C を線分に射影する
	else
	{
		// p を計算する
		float scalar = Vector3::Dot(ac, ab) / Vector3::Dot(ab, ab);
		Vector3 p = scalar * ab;

		// (ac - p) の2乗の長さを計算する(高価な平方根の演算を回避できる)
		return (ac - p).LengthSq();
	}
}

float LineSegment::MinDistSq(const LineSegment& s1, const LineSegment& s2)
{
	Vector3   u = s1.mEnd - s1.mStart;
	Vector3   v = s2.mEnd - s2.mStart;
	Vector3   w = s1.mStart - s2.mStart;
	float    a = Vector3::Dot(u, u);         // always >= 0
	float    b = Vector3::Dot(u, v);
	float    c = Vector3::Dot(v, v);         // always >= 0
	float    d = Vector3::Dot(u, w);
	float    e = Vector3::Dot(v, w);
	float    D = a * c - b * b;        // always >= 0
	float    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
	float    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0

    // 最も近い2つのポイントのラインパラメータを計算する
	// 線はほぼ平行なら
	if (Math::NearZero(D)) 
	{ 
		sN = 0.0;         // セグメントS1の点P0を使用して力を加える
		sD = 1.0;         // 後で0.0で除算される可能性を防ぐため
		tN = e;
		tD = c;
	}
	else 
	{                 // 無限の線上で最も近い点を取得する
		sN = (b * e - c * d);
		tN = (a * e - b * d);
		if (sN < 0.0) {        // sc < 0 => the s=0 edge is visible
			sN = 0.0;
			tN = e;
			tD = c;
		}
		else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	if (tN < 0.0) {            // tc < 0 => the t=0 edge is visible
		tN = 0.0;
		// recompute sc for this edge
		if (-d < 0.0)
			sN = 0.0;
		else if (-d > a)
			sN = sD;
		else {
			sN = -d;
			sD = a;
		}
	}
	else if (tN > tD) {      // tc > 1  => the t=1 edge is visible
		tN = tD;
		// recompute sc for this edge
		if ((-d + b) < 0.0)
			sN = 0;
		else if ((-d + b) > a)
			sN = sD;
		else {
			sN = (-d + b);
			sD = a;
		}
	}
	// finally do the division to get sc and tc
	sc = (Math::NearZero(sN) ? 0.0f : sN / sD);
	tc = (Math::NearZero(tN) ? 0.0f : tN / tD);

	// get the difference of the two closest points
	Vector3   dP = w + (sc * u) - (tc * v);  // =  S1(sc) - S2(tc)

	return dP.LengthSq();   // return the closest distance squared
}

Plane::Plane(const Vector3& normal, float d)
	:mNormal(normal)
	, mD(d)
{
}

// 3つの点から平面を構築する
Plane::Plane(const Vector3& a, const Vector3& b, const Vector3& c)
{
	// ab と ac のベクトルを計算
	Vector3 ab = b - a;
	Vector3 ac = c - a;

	// クロス積と正規化で法線を得る
	mNormal = Vector3::Cross(ab, ac);
	mNormal.Normalize();

	// dは、-P dot n
	mD = -Vector3::Dot(a, mNormal);
}

// 点と平面の符号付き最短距離を計算する
float Plane::SignedDist(const Vector3& point) const
{
	// 最短距離が負の値なら点は平面の法線と反対向きにある
    // 最短距離が正の値なら点は平面の法線と同じ向きにある
	return Vector3::Dot(point, mNormal) - mD;
}

Sphere::Sphere(const Vector3& center, float radius)
	:mCenter(center)
	, mRadius(radius)
{
}

// 球と点の包含判定
bool Sphere::Contains(const Vector3& point) const
{
	// 中心と点との距離の2乗を求める
	float distSq = (mCenter - point).LengthSq();
	return distSq <= (mRadius * mRadius);
}

AABB::AABB(const Vector3& min, const Vector3& max)
	: mMin(min)
	, mMax(max)
{
}

// 受け取った点の値に応じて最小点minと最大点maxを更新する
void AABB::UpdateMinMax(const Vector3& point)
{
	// 各成分を独立して分割する
	mMin.x = Math::Min(mMin.x, point.x);
	mMin.y = Math::Min(mMin.y, point.y);
	mMin.z = Math::Min(mMin.z, point.z);

	mMax.x = Math::Max(mMax.x, point.x);
	mMax.y = Math::Max(mMax.y, point.y);
	mMax.z = Math::Max(mMax.z, point.z);
}

// 指定した幅、高さ、奥行で最小点minと最大点maxを設定する
void AABB::SetMinMax(float width, float depth, float height)
{
	float wh = width / 2; 
	float dh = depth / 2;

	mMin = Vector3(-wh, -dh, 0);
	mMax = Vector3(wh, dh, height);
}

// オブジェクトの回転に合わせてAABBの寸法を変更する
void AABB::Rotate(const Quaternion& q)
{
	// ボックスの角の8点を構築
	std::array<Vector3, 8> points;

	// 最小の点は常に角にある
	points[0] = mMin;

	// 2個の min と1個の max による順列組み合わせ
	points[1] = Vector3(mMax.x, mMin.y, mMin.z);
	points[2] = Vector3(mMin.x, mMax.y, mMin.z);
	points[3] = Vector3(mMin.x, mMin.y, mMax.z);

	// 2個の max と1個の min による順列組み合わせ
	points[4] = Vector3(mMin.x, mMax.y, mMax.z);
	points[5] = Vector3(mMax.x, mMin.y, mMax.z);
	points[6] = Vector3(mMax.x, mMax.y, mMin.z);

	// 最大の点は角にある
	points[7] = Vector3(mMax);

	// 最初の点を回転する
	Vector3 p = Vector3::Transform(points[0], q);

	// 回転した第1の点で min/max をリセットする
	mMin = p;
	mMax = p;

	// 残りの（回転後の）点によって min/max を更新
	for (size_t i = 1; i < points.size(); i++)
	{
		p = Vector3::Transform(points[i], q);
		UpdateMinMax(p);
	}
}

// AABBの包含判定
bool AABB::Contains(const Vector3& point) const
{
	bool outside = point.x < mMin.x ||
		           point.y < mMin.y ||
		           point.z < mMin.z ||
		           point.x > mMax.x ||
		           point.y > mMax.y ||
		           point.z > mMax.z;

	// どれも真でなければ点はボックスの中にある
	return !outside;
}

// 点とAABBとの間の最短距離を計算する
float AABB::MinDistSq(const Vector3& point) const
{
	// 各軸の差を計算する
	float dx = Math::Max(mMin.x - point.x, 0.0f);
	dx = Math::Max(dx, point.x - mMax.x);
	float dy = Math::Max(mMin.y - point.y, 0.0f);
	dy = Math::Max(dy, point.y - mMax.y);
	float dz = Math::Max(mMin.z - point.z, 0.0f);
	dz = Math::Max(dy, point.z - mMax.z);

	// 距離の2乗を求める公式
	return dx * dx + dy * dy + dz * dz;
}

Capsule::Capsule(const Vector3& start, const Vector3& end, float radius)
	:mSegment(start, end)
	, mRadius(radius)
{
}

Vector3 Capsule::PointOnSegment(float t) const
{
	return mSegment.PointOnSegment(t);
}

// カプセルの包含判定
bool Capsule::Contains(const Vector3& point) const
{
	// 点と線分との最短距離の2乗を求める
	float distSq = mSegment.MinDistSq(point);
	return distSq <= (mRadius * mRadius);
}

// 凸ポリゴンの包含(2D)判定　※平方根とアークコサインを多用する為、効率は悪い
bool ConvexPolygon::Contains(const Vector2& point) const
{
	float sum = 0.0f;
	Vector2 a, b;

	for (size_t i = 0; i < mVertices.size() - 1; i++)
	{
		// 点から第1の頂点へのベクトル
		a = mVertices[i] - point;
		a.Normalize();

		// 点から第2の頂点へのベクトル
		b = mVertices[i + 1] - point;
		b.Normalize();

		// 二つのベクトルがなす角度を合計に加算
		sum += Math::Acos(Vector2::Dot(a, b));
	}

	// 最後と最初の頂点の間で角度を計算
	a = mVertices.back() - point;
	a.Normalize();
	b = mVertices.front() - point;
	b.Normalize();
	sum += Math::Acos(Vector2::Dot(a, b));

	// 合計が約２πであれば true を返す
	return Math::NearZero(sum - Math::TwoPi);
}

// 球と球の交差判定
bool Intersect(const Sphere& a, const Sphere& b)
{
	float distSq = (a.mCenter - b.mCenter).LengthSq();
	float sumRadii = a.mRadius + b.mRadius;
	return distSq <= (sumRadii * sumRadii);
}

// AABB同士の交差判定
bool Intersect(const AABB& a, const AABB& b)
{
	bool no = a.mMax.x < b.mMin.x ||
		      a.mMax.y < b.mMin.y ||
		      a.mMax.z < b.mMin.z ||
		      b.mMax.x < a.mMin.x ||
		      b.mMax.y < a.mMin.y ||
		      b.mMax.z < a.mMin.z;

	// これらのどれも真でなければ、AABB同士は交差している
	return !no;
}

// カプセル同士の交差判定
bool Intersect(const Capsule& a, const Capsule& b)
{
	// 互いの線分の間の最短距離の2乗を求める
	float distSq = LineSegment::MinDistSq(a.mSegment, b.mSegment);

	// distSqが半径の和の2乗以下であればカプセル同士は交差している
	float sumRadii = a.mRadius + b.mRadius;
	return distSq <= (sumRadii * sumRadii);
}

// 球とAABBとの交差判定
bool Intersect(const Sphere& s, const AABB& box)
{
	// 球の中心とAABBとの間の最短距離の2乗を求める
	float distSq = box.MinDistSq(s.mCenter);

	// distSqが球の半径の2乗を超えないのなら球とAABBは交差している
	return distSq <= (s.mRadius * s.mRadius);
}

// 線分と球の交差判定
bool Intersect(const LineSegment& l, const Sphere& s, float& outT)
{
	// 方程式に従ってX、Y、a、b、cを計算する
	Vector3 X = l.mStart - s.mCenter;
	Vector3 Y = l.mEnd - l.mStart;
	float a = Vector3::Dot(Y, Y);
	float b = 2.0f * Vector3::Dot(X, Y);
	float c = Vector3::Dot(X, X) - s.mRadius * s.mRadius;

	// 判別式を計算する
	float disc = b * b - 4.0f * a * c;
	if (disc < 0.0f)
	{
		return false;
	}
	else
	{
		disc = Math::Sqrt(disc);

		// tの解（min と max）を求める
		float tMin = (-b - disc) / (2.0f * a);
		float tMax = (-b + disc) / (2.0f * a);

		// tが線分の領域にあるのかチェック
		if (tMin >= 0.0f && tMin <= 1.0f)
		{
			outT = tMin;
			return true;
		}
		else if (tMax >= 0.0f && tMax <= 1.0f)
		{
			outT = tMax;
			return true;
		}
		else
		{
			return false;
		}
	}
}

// 線分と平面の交差判定
bool Intersect(const LineSegment& l, const Plane& p, float& outT)
{
	// 最初に t の解が存在するのかテストする()
	float denom = Vector3::Dot(l.mEnd - l.mStart, p.mNormal);

	if (Math::NearZero(denom))
	{
		// 交差の可能性があるのは唯一、start/endが平面上の点である時、
		// すなわち、（P dot N）== d の場合のみ
		if (Math::NearZero(Vector3::Dot(l.mStart, p.mNormal) - p.mD))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		float numer = -Vector3::Dot(l.mStart, p.mNormal) - p.mD;
		outT = numer / denom;

		// t が線分の境界内にあるか？
		if (outT >= 0.0f && outT <= 1.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

// AABBの側面が交差しているかテストするヘルパー関数
bool TestSidePlane(float start, float end, float negd, const Vector3& norm,
	std::vector<std::pair<float, Vector3>>& out)
{
	float denom = end - start;
	if (Math::NearZero(denom))
	{
		return false;
	}
	else
	{
		float numer = -start + negd;
		float t = numer / denom;

		// tが範囲内にあることをテストする
		if (t >= 0.0f && t <= 1.0f)
		{
			out.emplace_back(t, norm);
			return true;
		}
		else
		{
			return false;
		}
	}
}

// 線分とAABBの交差判定
bool Intersect(const LineSegment& l, const AABB& b, float& outT, Vector3& outNorm)
{
	// 可能性のあるtの値をすべて保存する配列
	std::vector<std::pair<float, Vector3>> tValues;

	// x 平面をテスト
	TestSidePlane(l.mStart.x, l.mEnd.x, b.mMin.x, Vector3::NegUnitX, tValues);
	TestSidePlane(l.mStart.x, l.mEnd.x, b.mMax.x, Vector3::UnitX, tValues);

	// y 平面をテスト
	TestSidePlane(l.mStart.y, l.mEnd.y, b.mMin.y, Vector3::NegUnitY, tValues);
	TestSidePlane(l.mStart.y, l.mEnd.y, b.mMax.y, Vector3::UnitY, tValues);

	// z 平面をテスト
	TestSidePlane(l.mStart.z, l.mEnd.z, b.mMin.z, Vector3::NegUnitZ, tValues);
	TestSidePlane(l.mStart.z, l.mEnd.z, b.mMax.z, Vector3::UnitZ, tValues);

	// t の値を小さい順にソート
	std::sort(tValues.begin(), tValues.end(), [](
		const std::pair<float, Vector3>& a,
		const std::pair<float, Vector3>& b) {
		return a.first < b.first;
	});
	// ボックスに交点が含まれるかテスト
	Vector3 point;
	for (auto& t : tValues)
	{
		point = l.PointOnSegment(t.first);
		if (b.Contains(point))
		{
			outT = t.first;
			outNorm = t.second;
			return true;
		}
	}

	// ボックスの内部に交点が1つも無いならfalseを返す
	return false;
}

// 球スイープの交差判定
bool SweptSphere(const Sphere& P0, const Sphere& P1,
	             const Sphere& Q0, const Sphere& Q1, float& outT)
{
	// 式のX, Y, a, b, cを計算
	Vector3 X = P0.mCenter - Q0.mCenter;
	Vector3 Y = P1.mCenter - P0.mCenter - (Q1.mCenter - Q0.mCenter);
	float a = Vector3::Dot(Y, Y);
	float b = 2.0f * Vector3::Dot(X, Y);
	float sumRadii = P0.mRadius + Q0.mRadius;
	float c = Vector3::Dot(X, X) - sumRadii * sumRadii;

	// 判別式を解く
	float disc = b * b - 4.0f * a * c;
	if (disc < 0.0f)
	{
		return false;
	}
	else
	{
		disc = Math::Sqrt(disc);

		// 小さいほうの解だけが重要
		outT = (-b - disc) / (2.0f * a);
		if (outT >= 0.0f && outT <= 0.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}