#pragma once
#include "Component.h"

// �v���g�^�C�v�錾
class Actor;

class TargetComponent : public Component
{
public:
	TargetComponent(Actor* owner);
	~TargetComponent();
};