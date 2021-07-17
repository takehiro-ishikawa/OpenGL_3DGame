#pragma once
#include "Component.h"
#include "Math.h"

// �v���g�^�C�v�錾
class Actor;
class Vector2;

class MoveComponent : public Component
{
public:
	// updateOrder�̒l���������قǁA��ɍX�V�����
	MoveComponent(Actor* owner, int updateOrder = 10);
	void Update(float deltaTime) override;

	Vector2 GetMoveSpeed() const { return mMoveSpeed; }
	float GetAngularSpeed() const { return mAngularSpeed; }
	void SetMoveSpeed(Vector2 speed) { mMoveSpeed = speed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	
protected:
	float mAngularSpeed; // ��]�𐧌䂷��(���W�A���l)
	Vector2 mMoveSpeed;  // �O�㍶�E�ړ��𐧌䂷��(�P�ʒ�/�b)
};
