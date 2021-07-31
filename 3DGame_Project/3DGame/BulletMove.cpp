#include "BulletMove.h"
#include "Actor.h"
#include "Character.h"
#include "Game.h"
#include "PhysWorld.h"
#include "TargetActor.h"
#include "Bullet.h"

BulletMove::BulletMove(Actor* owner, CharacterTag targetTag)
	:MoveComponent(owner)
	,mTargetTag(targetTag)
{

}

void BulletMove::Update(float deltaTime)
{
	// �i�s�����ɐ������\�z����
	const float segmentLength = mMoveSpeed.y * deltaTime;
	Vector3 start = mOwner->GetPosition();
	Vector3 dir = mOwner->GetForward();
	Vector3 end = start + dir * segmentLength;
	LineSegment l(start, end);

	// �\�z���������ƑS�Ẵ{�b�N�X�ƂŏՓ˔�����s��
	PhysWorld* phys = mOwner->GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;
	if (phys->SegmentCast(l, info) && info.mActor != mPlayer) // (�v���C���[�Ƃ͏Փ˂��Ȃ�)
	{
		Character* target = dynamic_cast<Character*>(info.mActor);
		// �L�����N�^�[�ɓ��������ꍇ
		if (target)
		{
			// ���������L�����N�^�[�����g�̕W�I�Ȃ�_���[�W��^����
			if(target->GetCharacterTag() == mTargetTag) target->Damage(1.0f);
		}

		// ���g�����ł�����
		mOwner->SetState(Actor::ActorState::EDead);
	}

	// ��{�N���X�̍X�V�́A�O�i���x�Ɋ�Â��Ĉړ����܂�
	MoveComponent::Update(deltaTime);
}