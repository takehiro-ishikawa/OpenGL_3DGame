#pragma once
#include "Component.h"

// プロトタイプ宣言
class Actor;

class TargetComponent : public Component
{
public:
	TargetComponent(Actor* owner);
	~TargetComponent();
};