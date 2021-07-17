#pragma once
#include "Math.h"
#include "Component.h"

// プロトタイプ宣言
class Actor;
class Shader;
class Mesh;

class PointLightComponent : public Component
{
public:
	PointLightComponent(Actor* owner);
	~PointLightComponent();

	// 点光源をジオメトリとして描画する
	void Draw(Shader* shader, Mesh* mesh);

	// 拡散色
	Vector3 mDiffuseColor;

	// 光の半径
	float mInnerRadius;
	float mOuterRadius;
};
