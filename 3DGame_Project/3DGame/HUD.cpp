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
	// ‘Ì—ÍƒQ[ƒW
	DrawHealthBar(shader);

	// FPS
	DrawFrameRate(shader);

	// ”wŒi‚Æ‘€ìƒKƒCƒh
	DrawTexture(shader, mBack, Vector2(0.0f, -359.0f));
	DrawTexture(shader, mBack, Vector2(0.0f, 359.0f));
	DrawTexture(shader, mGuideKeyBoard, Vector2(0.0f, -359.0f));
	DrawTexture(shader, mGuideController, Vector2(0.0f, 359.0f));
}

// ‘Ì—ÍƒQ[ƒW‚ð•`‰æ‚·‚é
void HUD::DrawHealthBar(Shader* shader)
{
	Vector2 pos = HEALTH_POSITION;

	// ‘Ì—ÍƒQ[ƒW‚Ì”wŒi‚Æ–{‘Ì‚ð•`‰æ
	DrawTexture(shader, mHealthBarBack, pos);
	DrawTexture(shader, mHealthBar, pos);

	// mNormalizeHealth‚Ì’l‚ÉŠî‚¢‚Ä‘Ì—ÍƒQ[ƒWŒ¸­•ª‚Ì‰æ‘œ‚ð•\Ž¦ˆÊ’u‚ÆƒTƒCƒY•‚ðŒˆ‚ß‚Ä•`‰æ
	float posX = Math::Lerp(pos.x, pos.x + mHealthBarEmpty->GetWidth() / 2, mNormalizeHealth);
	float width = Math::Lerp(1, 0, mNormalizeHealth);
	DrawTexture(shader, mHealthBarEmpty, Vector2(posX, pos.y), Vector2(width, 1));
}

// Œ»Ý‚ÌFPS‚ð•`‰æ‚·‚é
void HUD::DrawFrameRate(Shader* shader)
{
	int n = mGame->GetFrameRate();
	std::string s = "FPS:" + std::to_string(n);
	mFrameRate = mFont->RenderText(s, Color::Black, FPS_FONT_SIZE);
	DrawTexture(shader, mFrameRate, Vector2(FPS_POSITION));
}