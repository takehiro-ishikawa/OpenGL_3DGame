#pragma once
#include "Math.h"

// ボーンの最大値
const size_t MAX_SKELETON_BONES = 96;

// 行列パレット
struct MatrixPalette
{
	Matrix4 mEntry[MAX_SKELETON_BONES];
};
