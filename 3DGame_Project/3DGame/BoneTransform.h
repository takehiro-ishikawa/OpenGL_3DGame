#pragma once
#include "Math.h"

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