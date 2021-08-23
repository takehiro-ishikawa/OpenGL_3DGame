#pragma once
#include "Math.h"
#include "vector"
#include "string"
#include "Mesh.h"

// �v���g�^�C�v�錾
class FBXData;
class Actor;

// �{�[���ϊ����Ǘ�
class BoneTransform
{
public:
	// �{�[���̎����
	Vector3 mTranslation; // �ʒu
	Quaternion mRotation; // ����
	Matrix4 mMatrix;

	// �{�[���̏����s��ɕϊ�����
	Matrix4 ToMatrix() const;

	// ��̃{�[�����Ԃ���
	static BoneTransform Interpolate(const BoneTransform& a, const BoneTransform& b, float f);
};


// 1�̃X�P���g���f�[�^���Ǘ�
class Skeleton
{
public:
	// �t�@�C������ǂݍ���
	bool Load(FBXData* fbxFile);

	// �Q�b�^�[
	size_t GetNumBones() const { return mBones.size(); }
	const BoneTransform& GetBone(size_t idx) const { return mBones[idx]; }
	const std::vector<BoneTransform>& GetBones() const { return mBones; }
	const std::vector<Matrix4>& GetGlobalInvBindPoses() const { return mGlobalInvBindPoses; }

protected:
	// ���ׂẴ{�[���́A�O���[�o���ȋt�o�C���h�|�[�Y�s����v�Z����
	// �i�X�P���g���̃��[�h���ɌĂяo�����j
	void ComputeGlobalInvBindPose();

private:
	// �X�P���g���̃{�[���z��
	std::vector<BoneTransform> mBones;

	// �{�[���̃O���[�o���ȋt�o�C���h�|�[�Y�s��
	std::vector<Matrix4> mGlobalInvBindPoses;
};


// �A�j���[�V�����̏��
typedef struct
{
	size_t NumBones;     // �A�j���[�V�����̃{�[����
	size_t NumFrames;    // �A�j���[�V�����̃t���[����
	float Duration;      // �A�j���[�V�����̒����i�b�P�ʁj
	float FrameDuration; // �A�j���[�V�����̊e�t���[���̒���
}AnimInfo;

// 1�̃A�j���[�V�����f�[�^���Ǘ�
class Animation
{
public:
	bool Load(FBXData* fbxFile, const std::string& animationName);

	size_t GetNumBones() const { return mAnimInfo.NumBones; }
	size_t GetNumFrames() const { return mAnimInfo.NumFrames; }
	float GetDuration() const { return mAnimInfo.Duration; }
	float GetFrameDuration() const { return mAnimInfo.FrameDuration; }

	// �n���ꂽ�z��ɃO���[�o���|�[�Y�s����L������B
	// �w�肳�ꂽ�����ɂ�����A�e�{�[���̌��݂̃O���[�o���|�[�Y�̍s��ł���
	void GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const class Skeleton* inSkeleton, float inTime) const;

private:
	// �A�j���[�V�����̏��
	AnimInfo mAnimInfo;

	// �e�t���[���̕ϊ������g���b�N�Ɋi�[
	// �O���̔z��̃C���f�b�N�X�̓{�[��
	// �����̔z��̃C���f�b�N�X�̓t���[��
	std::vector<std::vector<BoneTransform>> mTracks;
};


// �{�[���̍ő�l
const size_t MAX_SKELETON_BONES = 96;

// �s��p���b�g
struct MatrixPalette
{
	Matrix4 mEntry[MAX_SKELETON_BONES];
};


// �X�P���^���A�j���[�V�������s�����b�V���̃R���|�[�l���g
class SkeletalMeshComponent : public MeshComponent
{
public:
	SkeletalMeshComponent(Actor* owner);

	// ���b�V���R���|�[�l���g��`��
	void Draw(Shader* shader) override;

	void Update(float deltaTime) override;

	// �Z�b�^�[
	void SetSkeleton(const Skeleton* sk) { mSkeleton = sk; }

	// �A�j���[�V�������Đ����B�A�j���[�V�����̒�����Ԃ�
	float PlayAnimation(const Animation* anim, float playRate = 1.0f);

protected:

	// �s��p���b�g���v�Z����
	void ComputeMatrixPalette();

	MatrixPalette mPalette;      // �s��p���b�g
	const Skeleton* mSkeleton;   // ���������蓖�Ă���X�P���g��
	const Animation* mAnimation; // ���Đ����Ă���A�j���[�V����
	float mAnimPlayRate;         // �A�j���[�V�����̍Đ����[�g�i1.0���ʏ�̃X�s�[�h�j
	float mAnimTime;             // �A�j���[�V�����̌��݂̎���
};