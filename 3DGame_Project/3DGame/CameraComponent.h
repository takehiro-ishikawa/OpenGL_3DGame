#pragma once
#include "Component.h"
#include "Math.h"

// �v���g�^�C�v�錾
class Actor;

class CameraComponent : public Component
{
public:
	CameraComponent(Actor* owner, int updateOrder = 200);

protected:
	void SetViewMatrix(const Matrix4& view);
};