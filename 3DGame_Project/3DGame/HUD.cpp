#include "HUD.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "PhysWorld.h"
#include "Player.h"
#include "Font.h"
#include <algorithm>

HUD::HUD(Game* game)
	:UIScreen(game)
	, mNormalizeHealth(1.0f)
{
	Renderer* r = mGame->GetRenderer();
	mHealthBarBack = r->GetTexture(HEALTHBAR_BACK_FILEPATH);
	mHealthBar = r->GetTexture(HEALTHBAR_FILEPATH);
	mHealthBarEmpty = r->GetTexture(HEALTHBAR_EMPTY_FILEPATH);

	mBack = r->GetTexture(HUD_BACK);
	mGuideKeyBoard = r->GetTexture(GUIDE_KEYBOARD);
	mGuideController = r->GetTexture(GUIDE_CONTROLLER);
}

HUD::~HUD()
{
}

void HUD::Update(float deltaTime)
{
	UIScreen::Update(deltaTime);
}

void HUD::Draw(Shader* shader)
{	
	// 体力ゲージ
	DrawHealthBar(shader);

	// FPS
	DrawFrameRate(shader);

	// 背景と操作ガイド
	DrawTexture(shader, mBack, Vector2(0.0f, -359.0f));
	DrawTexture(shader, mBack, Vector2(0.0f, 359.0f));
	DrawTexture(shader, mGuideKeyBoard, Vector2(0.0f, -359.0f));
	DrawTexture(shader, mGuideController, Vector2(0.0f, 359.0f));
}

// 体力ゲージを描画する
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

// 現在のFPSを描画する
void HUD::DrawFrameRate(Shader* shader)
{
	int n = mGame->GetFrameRate();
	std::string s = "FPS:" + std::to_string(n);
	mFrameRate = mFont->RenderText(s, Color::Black, FPS_FONT_SIZE);
	DrawTexture(shader, mFrameRate, Vector2(FPS_POSITION));
}