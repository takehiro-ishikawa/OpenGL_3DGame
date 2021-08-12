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
	//// レーダー
	//const Vector2 cRadarPos(-390.0f, 200.0f);
	//DrawTexture(shader, mRadar, cRadarPos);

	//// Blips
	//for (Vector2& blip : mBlips)
	//{
	//	DrawTexture(shader, mBlipTex, cRadarPos + blip);
	//}
	//// Radar arrow
	//DrawTexture(shader, mRadarArrow, cRadarPos);
	
	// 体力ゲージ
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

	// 体力ゲージの背景と本体を描画
	DrawTexture(shader, mHealthBarBack, pos);
	DrawTexture(shader, mHealthBar, pos);

	// mNormalizeHealthの値に基いて体力ゲージ減少分の画像を表示位置とサイズ幅を決めて描画
	float posX = Math::Lerp(pos.x, pos.x + mHealthBarEmpty->GetWidth() / 2, mNormalizeHealth);
	float width = Math::Lerp(1, 0, mNormalizeHealth);
	DrawTexture(shader, mHealthBarEmpty, Vector2(posX, pos.y), Vector2(width, 1));
}

void HUD::UpdateCrosshair(float deltaTime)
{
	// 通常のカーソルに戻す
	mTargetEnemy = false;

	// 線分の作成
	const float cAimDist = 5000.0f;
	Vector3 start, dir;
	mGame->GetRenderer()->GetScreenDirection(start, dir);
	LineSegment l(start, start + dir * cAimDist);

	// 線分キャスト
	PhysWorld::CollisionInfo info;
	if (mGame->GetPhysWorld()->SegmentCast(l, info))
	{
		// アクターはTargetCompornentを持っているか?
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
	// １つ前のフレームの輝点位置をクリア
	mBlips.clear();

	// プレイヤーの位置をレーダー座標に変換 (x が前方, z が上方)
	Vector3 playerPos = mGame->GetPlayer()->GetPosition();
	Vector2 playerPos2D(playerPos.y, playerPos.x);

	// 同様にプレイヤーの前方ベクトルを変換
	Vector3 playerForward = mGame->GetPlayer()->GetForward();
	Vector2 playerForward2D(playerForward.x, playerForward.y);

	// atan2を使ってレーダーの回転を求める
	float angle = Math::Atan2(playerForward2D.y, playerForward2D.x);
	// ２次元の回転行列を得る
	Matrix3 rotMat = Matrix3::CreateRotation(angle);

	// 輝点の位置を取得する
	for (auto tc : mTargetComps)
	{
		Vector3 targetPos = tc->GetOwner()->GetPosition();
		Vector2 actorPos2D(targetPos.y, targetPos.x);

		// プレイヤーからターゲットへのベクトルを計算
		Vector2 playerToTarget = actorPos2D - playerPos2D;

		// 範囲内にあるか？
		if (playerToTarget.LengthSq() <= (mRadarRange * mRadarRange))
		{
			// playerToTarget を、画面上のレーダーの
			// 中心からのオフセットに変換する
			Vector2 blipPos = playerToTarget;
			blipPos *= mRadarRadius / mRadarRange;

			// blipPos を回転する
			blipPos = Vector2::Transform(blipPos, rotMat);
			mBlips.emplace_back(blipPos);
		}
	}
}