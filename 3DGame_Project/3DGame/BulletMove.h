#pragma once
#include "MoveComponent.h"
#include "Character.h"
#include <functional>

#pragma region �v���g�^�C�v�錾
class Actor;
#pragma endregion

class BulletMove : public MoveComponent
{
public:
	BulletMove(Actor* owner, Actor* bulletOwner, CharacterTag targetTag, std::function<void(Character*)> onHit);

	void Update(float deltaTime) override;

	// �Q�b�^�[/�Z�b�^�[
	CharacterTag GetTargetTag() { return mTargetTag; }
	void SetTargetTag(CharacterTag tag) { mTargetTag = tag; }

protected:
	// �������ɌĂ΂��R�[���o�b�N
	std::function<void(Character*)> mOnHit;

	Actor* mBulletOwner;     // �e�𔭎˂����A�N�^�[
	CharacterTag mTargetTag; // �U���Ώۂ̃^�O
};