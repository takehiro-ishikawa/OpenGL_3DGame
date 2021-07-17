#pragma once
#include <vector>
#include <functional>
#include "Collision.h"

#pragma region �v���g�^�C�v�錾
class Game;
class BoxComponent;
class Actor;
#pragma endregion

// �S�Ă̓����蔻����Ǘ�
class PhysWorld
{
public:
	PhysWorld(Game* game);

	// �Փˌ��ʂɊւ���L�p�ȏ���񋟂��邽�߂Ɏg�p����
	struct CollisionInfo
	{
		// �Փ˂����ʒu
		Vector3 mPoint;

		// �Փ˓_�̖@��
		Vector3 mNormal;

		// ��������BoxComponent�ւ̃|�C���^
		BoxComponent* mBox;

		// ��������BoxComponent�̏��L�A�N�^�[�ւ̃|�C���^
		Actor* mActor;
	};

	// �󂯎���������ƑS�Ẵ{�b�N�X�Ƃ̌���������s��
	// �{�b�N�X�ɏՓ˂����ꍇ��true��Ԃ�
	bool SegmentCast(const LineSegment& l, CollisionInfo& outColl);
	bool SegmentCast(const LineSegment& l, CollisionInfo& outColl, Actor* myActor);

	// ���[���h�ɂ���S�Ẵ{�b�N�X�Ɋւ��ďՓ˂��e�X�g����
	void TestPairwise(std::function<void(Actor*, Actor*)> f);
	// ����̎��ɉ����ċ�Ԃ��d�Ȃ�{�b�N�X�����Փ˂��e�X�g����
	// �u��̃{�b�N�X�́A1�̍��W���ŋ�Ԃ��d�Ȃ荇��Ȃ���Ό������Ȃ��v�Ƃ��������𗘗p����
	void TestSweepAndPrune(std::function<void(Actor*, Actor*)> f);

	// ���[���h�Ƀ{�b�N�X�R���|�[�l���g��ǉ�/�폜
	void AddBox(BoxComponent* box);
	void RemoveBox(BoxComponent* box);

	const std::vector<BoxComponent*>& GetBoxes() { return mBoxes; }

private:
	Game* mGame;
	std::vector<BoxComponent*> mBoxes;
};