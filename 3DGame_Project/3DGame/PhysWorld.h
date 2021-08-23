#pragma once
#include <vector>
#include <functional>
#include "Math.h"

#pragma region �v���g�^�C�v�錾
class Game;
class BoxComponent;
class Actor;
#pragma endregion


// ����
struct LineSegment
{
	LineSegment(const Vector3& start, const Vector3& end);

	// 0 <= t <= 1�ł���Z�O�����g�ɉ����ă|�C���g���擾
	Vector3 PointOnSegment(float t) const;

	// ���W�Ɛ����̊Ԃ̍ŒZ������2����擾
	float MinDistSq(const Vector3& point) const;

	// �����Ɛ����Ƃ̊Ԃ̍ŒZ�������v�Z����
	static float MinDistSq(const LineSegment& s1, const LineSegment& s2);

	Vector3 mStart; // �n�_
	Vector3 mEnd;   // �I�_
};

// ����
struct Plane
{
	Plane(const Vector3& normal, float d);

	// 3�_���畽�ʂ��\�z����
	Plane(const Vector3& a, const Vector3& b, const Vector3& c);

	// �_�ƕ��ʂ̊Ԃ̕����t���������擾
	float SignedDist(const Vector3& point) const;

	Vector3 mNormal; // �@��
	float mD;        // ���ʂƌ��_�Ƃ̕����t��(�ŒZ)����
};

// ��
struct Sphere
{
	Sphere(const Vector3& center, float radius);
	bool Contains(const Vector3& point) const;

	Vector3 mCenter; // ���S�ʒu
	float mRadius;   // ���a
};

// �����s�o�E���f�B���O�{�b�N�X�i�ǂ̕ӂ�X����Y���ɕ��s�Ȕ��j
struct AABB
{
	AABB(const Vector3& min, const Vector3& max);

	// �󂯎�����_�̒l�ɉ����čŏ��_min�ƍő�_max���X�V����i���f���̃��[�h���Ɏg�p�����j
	void UpdateMinMax(const Vector3& point);

	// �w�肵�����A�����A���s�ōŏ��_min�ƍő�_max��ݒ肷��
	void SetMinMax(float width, float depth, float height);

	// �I�u�W�F�N�g�̉�]�ɍ��킹��AABB�̐��@��ύX����
	void Rotate(const Quaternion& q);

	// AABB�̕�ܔ���
	bool Contains(const Vector3& point) const;

	// �_��AABB�Ƃ̊Ԃ̍ŒZ�������v�Z����
	float MinDistSq(const Vector3& point) const;

	Vector3 mMin; // �ŏ��̓_
	Vector3 mMax; // �ő�̓_
};

// �L���o�E���f�B���O�{�b�N�X�i�����s�̐����������Ȃ����j
struct OBB
{
	Vector3 mCenter;      // ���S�̈ʒu
	Quaternion mRotation; // ��]�N�H�[�^�j�I��
	Vector3 mExtents;     // �{�b�N�X�̍L����
};

// �J�v�Z��
struct Capsule
{
	Capsule(const Vector3& start, const Vector3& end, float radius);

	// 0 <= t <= 1�ł���Z�O�����g�ɉ����ă|�C���g���擾
	Vector3 PointOnSegment(float t) const;

	bool Contains(const Vector3& point) const;

	LineSegment mSegment; // ����
	float mRadius;        // ���a
};

// �ʃ|���S��
struct ConvexPolygon
{
	bool Contains(const Vector2& point) const;

	// ���_�ɂ͎��v���̏���������
	std::vector<Vector2> mVertices;
};

// ��������̊֐�
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


// �S�Ă̓����蔻����Ǘ�
class PhysWorld
{
public:
	PhysWorld(Game* game);

	// �Փˌ��ʂɊւ���L�p�ȏ���񋟂��邽�߂Ɏg�p����
	struct CollisionInfo
	{
		// �Փ˂����ʒu
		Vector3 mPoint;

		// �Փ˓_�̖@��
		Vector3 mNormal;

		// ��������BoxComponent�ւ̃|�C���^
		BoxComponent* mBox;

		// ��������BoxComponent�̏��L�A�N�^�[�ւ̃|�C���^
		Actor* mActor;
	};

	// �󂯎���������ƑS�Ẵ{�b�N�X�Ƃ̌���������s��
	// �{�b�N�X�ɏՓ˂����ꍇ��true��Ԃ�
	bool SegmentCast(const LineSegment& l, CollisionInfo& outColl);
	bool SegmentCast(const LineSegment& l, CollisionInfo& outColl, Actor* myActor);

	// ���[���h�ɂ���S�Ẵ{�b�N�X�Ɋւ��ďՓ˂��e�X�g����
	void TestPairwise(std::function<void(Actor*, Actor*)> f);
	// ����̎��ɉ����ċ�Ԃ��d�Ȃ�{�b�N�X�����Փ˂��e�X�g����
	// �u��̃{�b�N�X�́A1�̍��W���ŋ�Ԃ��d�Ȃ荇��Ȃ���Ό������Ȃ��v�Ƃ��������𗘗p����
	void TestSweepAndPrune(std::function<void(Actor*, Actor*)> f);

	// ���[���h�Ƀ{�b�N�X�R���|�[�l���g��ǉ�/�폜
	void AddBox(BoxComponent* box);
	void RemoveBox(BoxComponent* box);

	const std::vector<BoxComponent*>& GetBoxes() { return mBoxes; }

private:
	Game* mGame;
	std::vector<BoxComponent*> mBoxes;
};