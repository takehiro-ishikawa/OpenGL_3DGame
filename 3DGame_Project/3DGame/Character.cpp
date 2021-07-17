#include "Character.h"
#include "Game.h"
#include "PhysWorld.h"
#include "BoxComponent.h"
#include "AudioComponent.h"
#include "PlaneActor.h"


Character::Character(Game* game)
	:Actor(game)
{
	mAudioComp = new AudioComponent(this);
	mBoxComp = new BoxComponent(this);
}

void Character::FixCollisions()
{
	// ���[���h��Ԃ̃{�b�N�X���X�V���邽�߂ɁA�����̃��[���h�ϊ����Čv�Z����K�v������
	ComputeWorldTransform();

	// ���g��AABB���擾
	const AABB& characterBox = mBoxComp->GetWorldBox();
	Vector3 pos = GetPosition();

	// ���g�ȊO�S�Ă�AABB�ƐڐG������s��
	auto& boxes = GetGame()->GetPhysWorld()->GetBoxes();
	for (auto box : boxes)
	{
		if (box->GetOwner() == this) continue;

		// ���̃{�b�N�X�ƏՓ˂��邩?
		const AABB& boxAABB = box->GetWorldBox();
		if (Intersect(characterBox, boxAABB))
		{
			// �S�Ă̍����v�Z����
			float dx1 = boxAABB.mMax.x - characterBox.mMin.x;
			float dx2 = boxAABB.mMin.x - characterBox.mMax.x;
			float dy1 = boxAABB.mMax.y - characterBox.mMin.y;
			float dy2 = boxAABB.mMin.y - characterBox.mMax.y;
			float dz1 = boxAABB.mMax.z - characterBox.mMin.z;
			float dz2 = boxAABB.mMin.z - characterBox.mMax.z;

			// dx�ɂ́Adx1/dx2�̂�����Βl���������ق����Z�b�g����
			float dx = Math::Abs(dx1) < Math::Abs(dx2) ? dx1 : dx2;
			// dy�����l
			float dy = Math::Abs(dy1) < Math::Abs(dy2) ? dy1 : dy2;
			// dz�����l
			float dz = Math::Abs(dz1) < Math::Abs(dz2) ? dz1 : dz2;

			// x/y/z�̂����ł��������������ňʒu�𒲐�
			if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
			{
				pos.x += dx;
			}
			else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
			{
				pos.y += dy;
			}
			else
			{
				pos.z += dz;
			}

			// �ʒu��ݒ肵�{�b�N�X�̐������X�V����
			SetPosition(pos);
			mBoxComp->OnUpdateWorldTransform();
		}
	}
}