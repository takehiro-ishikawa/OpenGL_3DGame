#pragma once
#include "Math.h"
#include "Component.h"

// �v���g�^�C�v�錾
class Actor;
class Shader;
class Mesh;

class PointLightComponent : public Component
{
public:
	PointLightComponent(Actor* owner);
	~PointLightComponent();

	// �_�������W�I���g���Ƃ��ĕ`�悷��
	void Draw(Shader* shader, Mesh* mesh);

	// �g�U�F
	Vector3 mDiffuseColor;

	// ���̔��a
	float mInnerRadius;
	float mOuterRadius;
};
