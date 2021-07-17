#pragma once
#include "Math.h"

// ボーン変換を管理
class BoneTransform
{
public:
	// ボーンの持つ情報
	Vector3 mTranslation; // 位置
	Quaternion mRotation; // 方向
	Matrix4 mMatrix;

	// ボーンの情報を行列に変換する
	Matrix4 ToMatrix() const;

	// 二つのボーンを補間する
	static BoneTransform Interpolate(const BoneTransform& a, const BoneTransform& b, float f);
};