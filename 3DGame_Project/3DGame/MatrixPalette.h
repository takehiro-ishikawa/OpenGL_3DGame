#pragma once
#include "Math.h"

// �{�[���̍ő�l
const size_t MAX_SKELETON_BONES = 96;

// �s��p���b�g
struct MatrixPalette
{
	Matrix4 mEntry[MAX_SKELETON_BONES];
};
