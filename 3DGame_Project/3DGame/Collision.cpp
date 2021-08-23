#include "PhysWorld.h"
#include <algorithm>
#include <array>

LineSegment::LineSegment(const Vector3& start, const Vector3& end)
	:mStart(start)
	, mEnd(end)
{
}

// t����͂Ƃ��Đ�����̓_��Ԃ�
Vector3 LineSegment::PointOnSegment(float t) const
{
	return mStart + (mEnd - mStart) * t;
}

// �_�Ɛ����Ƃ̊Ԃ̍ŒZ�������v�Z����
float LineSegment::MinDistSq(const Vector3& point) const
{
	// �x�N�g��������
	Vector3 ab = mEnd - mStart;
	Vector3 ba = -1.0f * ab;
	Vector3 ac = point - mStart;
	Vector3 bc = point - mEnd;

	// Case 1: C �� A �̑O�ɓ˂��o�Ă���
	if (Vector3::Dot(ab, ac) < 0.0f)
	{
		return ac.LengthSq();
	}
	// Case 2: C �� B �̂��Ƃɓ˂��o�Ă���
	else if (Vector3::Dot(ba, bc) < 0.0f)
	{
		return bc.LengthSq();
	}
	// Case 3: C ������Ɏˉe����
	else
	{
		// p ���v�Z����
		float scalar = Vector3::Dot(ac, ab) / Vector3::Dot(ab, ab);
		Vector3 p = scalar * ab;

		// (ac - p) ��2��̒������v�Z����(�����ȕ������̉��Z������ł���)
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

    // �ł��߂�2�̃|�C���g�̃��C���p�����[�^���v�Z����
	// ���͂قڕ��s�Ȃ�
	if (Math::NearZero(D)) 
	{ 
		sN = 0.0;         // �Z�O�����gS1�̓_P0���g�p���ė͂�������
		sD = 1.0;         // ���0.0�ŏ��Z�����\����h������
		tN = e;
		tD = c;
	}
	else 
	{                 // �����̐���ōł��߂��_���擾����
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

// 3�̓_���畽�ʂ��\�z����
Plane::Plane(const Vector3& a, const Vector3& b, const Vector3& c)
{
	// ab �� ac �̃x�N�g�����v�Z
	Vector3 ab = b - a;
	Vector3 ac = c - a;

	// �N���X�ςƐ��K���Ŗ@���𓾂�
	mNormal = Vector3::Cross(ab, ac);
	mNormal.Normalize();

	// d�́A-P dot n
	mD = -Vector3::Dot(a, mNormal);
}

// �_�ƕ��ʂ̕����t���ŒZ�������v�Z����
float Plane::SignedDist(const Vector3& point) const
{
	// �ŒZ���������̒l�Ȃ�_�͕��ʂ̖@���Ɣ��Ό����ɂ���
    // �ŒZ���������̒l�Ȃ�_�͕��ʂ̖@���Ɠ��������ɂ���
	return Vector3::Dot(point, mNormal) - mD;
}

Sphere::Sphere(const Vector3& center, float radius)
	:mCenter(center)
	, mRadius(radius)
{
}

// ���Ɠ_�̕�ܔ���
bool Sphere::Contains(const Vector3& point) const
{
	// ���S�Ɠ_�Ƃ̋�����2������߂�
	float distSq = (mCenter - point).LengthSq();
	return distSq <= (mRadius * mRadius);
}

AABB::AABB(const Vector3& min, const Vector3& max)
	: mMin(min)
	, mMax(max)
{
}

// �󂯎�����_�̒l�ɉ����čŏ��_min�ƍő�_max���X�V����
void AABB::UpdateMinMax(const Vector3& point)
{
	// �e������Ɨ����ĕ�������
	mMin.x = Math::Min(mMin.x, point.x);
	mMin.y = Math::Min(mMin.y, point.y);
	mMin.z = Math::Min(mMin.z, point.z);

	mMax.x = Math::Max(mMax.x, point.x);
	mMax.y = Math::Max(mMax.y, point.y);
	mMax.z = Math::Max(mMax.z, point.z);
}

// �w�肵�����A�����A���s�ōŏ��_min�ƍő�_max��ݒ肷��
void AABB::SetMinMax(float width, float depth, float height)
{
	float wh = width / 2; 
	float dh = depth / 2;

	mMin = Vector3(-wh, -dh, 0);
	mMax = Vector3(wh, dh, height);
}

// �I�u�W�F�N�g�̉�]�ɍ��킹��AABB�̐��@��ύX����
void AABB::Rotate(const Quaternion& q)
{
	// �{�b�N�X�̊p��8�_���\�z
	std::array<Vector3, 8> points;

	// �ŏ��̓_�͏�Ɋp�ɂ���
	points[0] = mMin;

	// 2�� min ��1�� max �ɂ�鏇��g�ݍ��킹
	points[1] = Vector3(mMax.x, mMin.y, mMin.z);
	points[2] = Vector3(mMin.x, mMax.y, mMin.z);
	points[3] = Vector3(mMin.x, mMin.y, mMax.z);

	// 2�� max ��1�� min �ɂ�鏇��g�ݍ��킹
	points[4] = Vector3(mMin.x, mMax.y, mMax.z);
	points[5] = Vector3(mMax.x, mMin.y, mMax.z);
	points[6] = Vector3(mMax.x, mMax.y, mMin.z);

	// �ő�̓_�͊p�ɂ���
	points[7] = Vector3(mMax);

	// �ŏ��̓_����]����
	Vector3 p = Vector3::Transform(points[0], q);

	// ��]������1�̓_�� min/max �����Z�b�g����
	mMin = p;
	mMax = p;

	// �c��́i��]��́j�_�ɂ���� min/max ���X�V
	for (size_t i = 1; i < points.size(); i++)
	{
		p = Vector3::Transform(points[i], q);
		UpdateMinMax(p);
	}
}

// AABB�̕�ܔ���
bool AABB::Contains(const Vector3& point) const
{
	bool outside = point.x < mMin.x ||
		           point.y < mMin.y ||
		           point.z < mMin.z ||
		           point.x > mMax.x ||
		           point.y > mMax.y ||
		           point.z > mMax.z;

	// �ǂ���^�łȂ���Γ_�̓{�b�N�X�̒��ɂ���
	return !outside;
}

// �_��AABB�Ƃ̊Ԃ̍ŒZ�������v�Z����
float AABB::MinDistSq(const Vector3& point) const
{
	// �e���̍����v�Z����
	float dx = Math::Max(mMin.x - point.x, 0.0f);
	dx = Math::Max(dx, point.x - mMax.x);
	float dy = Math::Max(mMin.y - point.y, 0.0f);
	dy = Math::Max(dy, point.y - mMax.y);
	float dz = Math::Max(mMin.z - point.z, 0.0f);
	dz = Math::Max(dy, point.z - mMax.z);

	// ������2������߂����
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

// �J�v�Z���̕�ܔ���
bool Capsule::Contains(const Vector3& point) const
{
	// �_�Ɛ����Ƃ̍ŒZ������2������߂�
	float distSq = mSegment.MinDistSq(point);
	return distSq <= (mRadius * mRadius);
}

// �ʃ|���S���̕��(2D)����@���������ƃA�[�N�R�T�C���𑽗p����ׁA�����͈���
bool ConvexPolygon::Contains(const Vector2& point) const
{
	float sum = 0.0f;
	Vector2 a, b;

	for (size_t i = 0; i < mVertices.size() - 1; i++)
	{
		// �_�����1�̒��_�ւ̃x�N�g��
		a = mVertices[i] - point;
		a.Normalize();

		// �_�����2�̒��_�ւ̃x�N�g��
		b = mVertices[i + 1] - point;
		b.Normalize();

		// ��̃x�N�g�����Ȃ��p�x�����v�ɉ��Z
		sum += Math::Acos(Vector2::Dot(a, b));
	}

	// �Ō�ƍŏ��̒��_�̊ԂŊp�x���v�Z
	a = mVertices.back() - point;
	a.Normalize();
	b = mVertices.front() - point;
	b.Normalize();
	sum += Math::Acos(Vector2::Dot(a, b));

	// ���v����Q�΂ł���� true ��Ԃ�
	return Math::NearZero(sum - Math::TwoPi);
}

// ���Ƌ��̌�������
bool Intersect(const Sphere& a, const Sphere& b)
{
	float distSq = (a.mCenter - b.mCenter).LengthSq();
	float sumRadii = a.mRadius + b.mRadius;
	return distSq <= (sumRadii * sumRadii);
}

// AABB���m�̌�������
bool Intersect(const AABB& a, const AABB& b)
{
	bool no = a.mMax.x < b.mMin.x ||
		      a.mMax.y < b.mMin.y ||
		      a.mMax.z < b.mMin.z ||
		      b.mMax.x < a.mMin.x ||
		      b.mMax.y < a.mMin.y ||
		      b.mMax.z < a.mMin.z;

	// �����̂ǂ���^�łȂ���΁AAABB���m�͌������Ă���
	return !no;
}

// �J�v�Z�����m�̌�������
bool Intersect(const Capsule& a, const Capsule& b)
{
	// �݂��̐����̊Ԃ̍ŒZ������2������߂�
	float distSq = LineSegment::MinDistSq(a.mSegment, b.mSegment);

	// distSq�����a�̘a��2��ȉ��ł���΃J�v�Z�����m�͌������Ă���
	float sumRadii = a.mRadius + b.mRadius;
	return distSq <= (sumRadii * sumRadii);
}

// ����AABB�Ƃ̌�������
bool Intersect(const Sphere& s, const AABB& box)
{
	// ���̒��S��AABB�Ƃ̊Ԃ̍ŒZ������2������߂�
	float distSq = box.MinDistSq(s.mCenter);

	// distSq�����̔��a��2��𒴂��Ȃ��̂Ȃ狅��AABB�͌������Ă���
	return distSq <= (s.mRadius * s.mRadius);
}

// �����Ƌ��̌�������
bool Intersect(const LineSegment& l, const Sphere& s, float& outT)
{
	// �������ɏ]����X�AY�Aa�Ab�Ac���v�Z����
	Vector3 X = l.mStart - s.mCenter;
	Vector3 Y = l.mEnd - l.mStart;
	float a = Vector3::Dot(Y, Y);
	float b = 2.0f * Vector3::Dot(X, Y);
	float c = Vector3::Dot(X, X) - s.mRadius * s.mRadius;

	// ���ʎ����v�Z����
	float disc = b * b - 4.0f * a * c;
	if (disc < 0.0f)
	{
		return false;
	}
	else
	{
		disc = Math::Sqrt(disc);

		// t�̉��imin �� max�j�����߂�
		float tMin = (-b - disc) / (2.0f * a);
		float tMax = (-b + disc) / (2.0f * a);

		// t�������̗̈�ɂ���̂��`�F�b�N
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

// �����ƕ��ʂ̌�������
bool Intersect(const LineSegment& l, const Plane& p, float& outT)
{
	// �ŏ��� t �̉������݂���̂��e�X�g����()
	float denom = Vector3::Dot(l.mEnd - l.mStart, p.mNormal);

	if (Math::NearZero(denom))
	{
		// �����̉\��������̂͗B��Astart/end�����ʏ�̓_�ł��鎞�A
		// ���Ȃ킿�A�iP dot N�j== d �̏ꍇ�̂�
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

		// t �������̋��E���ɂ��邩�H
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

// AABB�̑��ʂ��������Ă��邩�e�X�g����w���p�[�֐�
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

		// t���͈͓��ɂ��邱�Ƃ��e�X�g����
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

// ������AABB�̌�������
bool Intersect(const LineSegment& l, const AABB& b, float& outT, Vector3& outNorm)
{
	// �\���̂���t�̒l�����ׂĕۑ�����z��
	std::vector<std::pair<float, Vector3>> tValues;

	// x ���ʂ��e�X�g
	TestSidePlane(l.mStart.x, l.mEnd.x, b.mMin.x, Vector3::NegUnitX, tValues);
	TestSidePlane(l.mStart.x, l.mEnd.x, b.mMax.x, Vector3::UnitX, tValues);

	// y ���ʂ��e�X�g
	TestSidePlane(l.mStart.y, l.mEnd.y, b.mMin.y, Vector3::NegUnitY, tValues);
	TestSidePlane(l.mStart.y, l.mEnd.y, b.mMax.y, Vector3::UnitY, tValues);

	// z ���ʂ��e�X�g
	TestSidePlane(l.mStart.z, l.mEnd.z, b.mMin.z, Vector3::NegUnitZ, tValues);
	TestSidePlane(l.mStart.z, l.mEnd.z, b.mMax.z, Vector3::UnitZ, tValues);

	// t �̒l�����������Ƀ\�[�g
	std::sort(tValues.begin(), tValues.end(), [](
		const std::pair<float, Vector3>& a,
		const std::pair<float, Vector3>& b) {
		return a.first < b.first;
	});
	// �{�b�N�X�Ɍ�_���܂܂�邩�e�X�g
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

	// �{�b�N�X�̓����Ɍ�_��1�������Ȃ�false��Ԃ�
	return false;
}

// ���X�C�[�v�̌�������
bool SweptSphere(const Sphere& P0, const Sphere& P1,
	             const Sphere& Q0, const Sphere& Q1, float& outT)
{
	// ����X, Y, a, b, c���v�Z
	Vector3 X = P0.mCenter - Q0.mCenter;
	Vector3 Y = P1.mCenter - P0.mCenter - (Q1.mCenter - Q0.mCenter);
	float a = Vector3::Dot(Y, Y);
	float b = 2.0f * Vector3::Dot(X, Y);
	float sumRadii = P0.mRadius + Q0.mRadius;
	float c = Vector3::Dot(X, X) - sumRadii * sumRadii;

	// ���ʎ�������
	float disc = b * b - 4.0f * a * c;
	if (disc < 0.0f)
	{
		return false;
	}
	else
	{
		disc = Math::Sqrt(disc);

		// �������ق��̉��������d�v
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