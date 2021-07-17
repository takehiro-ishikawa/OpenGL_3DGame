#pragma once
#include "Component.h"
#include "Math.h"

// プロトタイプ宣言
class Actor;

class CameraComponent : public Component
{
public:
	CameraComponent(Actor* owner, int updateOrder = 200);

protected:
	void SetViewMatrix(const Matrix4& view);
};