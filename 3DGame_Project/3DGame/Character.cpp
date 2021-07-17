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
	// ワールド空間のボックスを更新するために、自分のワールド変換を再計算する必要がある
	ComputeWorldTransform();

	// 自身のAABBを取得
	const AABB& characterBox = mBoxComp->GetWorldBox();
	Vector3 pos = GetPosition();

	// 自身以外全てのAABBと接触判定を行う
	auto& boxes = GetGame()->GetPhysWorld()->GetBoxes();
	for (auto box : boxes)
	{
		if (box->GetOwner() == this) continue;

		// このボックスと衝突するか?
		const AABB& boxAABB = box->GetWorldBox();
		if (Intersect(characterBox, boxAABB))
		{
			// 全ての差を計算する
			float dx1 = boxAABB.mMax.x - characterBox.mMin.x;
			float dx2 = boxAABB.mMin.x - characterBox.mMax.x;
			float dy1 = boxAABB.mMax.y - characterBox.mMin.y;
			float dy2 = boxAABB.mMin.y - characterBox.mMax.y;
			float dz1 = boxAABB.mMax.z - characterBox.mMin.z;
			float dz2 = boxAABB.mMin.z - characterBox.mMax.z;

			// dxには、dx1/dx2のうち絶対値が小さいほうをセットする
			float dx = Math::Abs(dx1) < Math::Abs(dx2) ? dx1 : dx2;
			// dyも同様
			float dy = Math::Abs(dy1) < Math::Abs(dy2) ? dy1 : dy2;
			// dzも同様
			float dz = Math::Abs(dz1) < Math::Abs(dz2) ? dz1 : dz2;

			// x/y/zのうち最も差が小さい軸で位置を調整
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

			// 位置を設定しボックスの成分を更新する
			SetPosition(pos);
			mBoxComp->OnUpdateWorldTransform();
		}
	}
}