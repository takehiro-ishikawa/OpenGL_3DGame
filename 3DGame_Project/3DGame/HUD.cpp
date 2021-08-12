#include "HUD.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "PhysWorld.h"
#include "Player.h"
#include <algorithm>
#include "TargetComponent.h"

HUD::HUD(Game* game)
	:UIScreen(game)
	, mNormalizeHealth(1.0f)
	, mRadarRange(2000.0f)
	, mRadarRadius(92.0f)
	, mTargetEnemy(false)
{
	Renderer* r = mGame->GetRenderer();
	mHealthBarBack = r->GetTexture(HEALTHBAR_BACK_FILEPATH);
	mHealthBar = r->GetTexture(HEALTHBAR_FILEPATH);
	mHealthBarEmpty = r->GetTexture(HEALTHBAR_EMPTY_FILEPATH);
	mRadar = r->GetTexture("Assets/Textures/UI/Radar.png");
	mBlipTex = r->GetTexture("Assets/Textures/UI/Blip.png");
	mRadarArrow = r->GetTexture("Assets/Textures/UI/RadarArrow.png");

	mBack = r->GetTexture(HUD_BACK);
	mGuide = r->GetTexture("Assets/Textures/UI/Guide_Back.png");
}

HUD::~HUD()
{
}

void HUD::Update(float deltaTime)
{
	UIScreen::Update(deltaTime);

	UpdateCrosshair(deltaTime);
	UpdateRadar(deltaTime);
}

void HUD::Draw(Shader* shader)
{
	//// ���[�_�[
	//const Vector2 cRadarPos(-390.0f, 200.0f);
	//DrawTexture(shader, mRadar, cRadarPos);

	//// Blips
	//for (Vector2& blip : mBlips)
	//{
	//	DrawTexture(shader, mBlipTex, cRadarPos + blip);
	//}
	//// Radar arrow
	//DrawTexture(shader, mRadarArrow, cRadarPos);
	
	// �̗̓Q�[�W
	DrawHealthBar(shader);

	DrawTexture(shader, mBack, Vector2(0.0f, -359.0f));
	DrawTexture(shader, mBack, Vector2(0.0f, 359.0f));
	DrawTexture(shader, mGuide, Vector2(0.0f, -359.0f));
}

void HUD::AddTargetComponent(TargetComponent* tc)
{
	mTargetComps.emplace_back(tc);
}

void HUD::RemoveTargetComponent(TargetComponent* tc)
{
	auto iter = std::find(mTargetComps.begin(), mTargetComps.end(),
		tc);
	mTargetComps.erase(iter);
}

void HUD::DrawHealthBar(Shader* shader)
{
	Vector2 pos = HEALTH_POSITION;

	// �̗̓Q�[�W�̔w�i�Ɩ{�̂�`��
	DrawTexture(shader, mHealthBarBack, pos);
	DrawTexture(shader, mHealthBar, pos);

	// mNormalizeHealth�̒l�Ɋ�đ̗̓Q�[�W�������̉摜��\���ʒu�ƃT�C�Y�������߂ĕ`��
	float posX = Math::Lerp(pos.x, pos.x + mHealthBarEmpty->GetWidth() / 2, mNormalizeHealth);
	float width = Math::Lerp(1, 0, mNormalizeHealth);
	DrawTexture(shader, mHealthBarEmpty, Vector2(posX, pos.y), Vector2(width, 1));
}

void HUD::UpdateCrosshair(float deltaTime)
{
	// �ʏ�̃J�[�\���ɖ߂�
	mTargetEnemy = false;

	// �����̍쐬
	const float cAimDist = 5000.0f;
	Vector3 start, dir;
	mGame->GetRenderer()->GetScreenDirection(start, dir);
	LineSegment l(start, start + dir * cAimDist);

	// �����L���X�g
	PhysWorld::CollisionInfo info;
	if (mGame->GetPhysWorld()->SegmentCast(l, info))
	{
		// �A�N�^�[��TargetCompornent�������Ă��邩?
		for (auto tc : mTargetComps)
		{
			if (tc->GetOwner() == info.mActor)
			{
				mTargetEnemy = true;
				break;
			}
		}
	}
}

void HUD::UpdateRadar(float deltaTime)
{
	// �P�O�̃t���[���̋P�_�ʒu���N���A
	mBlips.clear();

	// �v���C���[�̈ʒu�����[�_�[���W�ɕϊ� (x ���O��, z �����)
	Vector3 playerPos = mGame->GetPlayer()->GetPosition();
	Vector2 playerPos2D(playerPos.y, playerPos.x);

	// ���l�Ƀv���C���[�̑O���x�N�g����ϊ�
	Vector3 playerForward = mGame->GetPlayer()->GetForward();
	Vector2 playerForward2D(playerForward.x, playerForward.y);

	// atan2���g���ă��[�_�[�̉�]�����߂�
	float angle = Math::Atan2(playerForward2D.y, playerForward2D.x);
	// �Q�����̉�]�s��𓾂�
	Matrix3 rotMat = Matrix3::CreateRotation(angle);

	// �P�_�̈ʒu���擾����
	for (auto tc : mTargetComps)
	{
		Vector3 targetPos = tc->GetOwner()->GetPosition();
		Vector2 actorPos2D(targetPos.y, targetPos.x);

		// �v���C���[����^�[�Q�b�g�ւ̃x�N�g�����v�Z
		Vector2 playerToTarget = actorPos2D - playerPos2D;

		// �͈͓��ɂ��邩�H
		if (playerToTarget.LengthSq() <= (mRadarRange * mRadarRange))
		{
			// playerToTarget ���A��ʏ�̃��[�_�[��
			// ���S����̃I�t�Z�b�g�ɕϊ�����
			Vector2 blipPos = playerToTarget;
			blipPos *= mRadarRadius / mRadarRange;

			// blipPos ����]����
			blipPos = Vector2::Transform(blipPos, rotMat);
			mBlips.emplace_back(blipPos);
		}
	}
}