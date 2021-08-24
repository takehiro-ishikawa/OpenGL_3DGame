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
	// �̗̓Q�[�W
	DrawHealthBar(shader);

	// FPS
	DrawFrameRate(shader);

	// �w�i�Ƒ���K�C�h
	DrawTexture(shader, mBack, Vector2(0.0f, -359.0f));
	DrawTexture(shader, mBack, Vector2(0.0f, 359.0f));
	DrawTexture(shader, mGuideKeyBoard, Vector2(0.0f, -359.0f));
	DrawTexture(shader, mGuideController, Vector2(0.0f, 359.0f));
}

// �̗̓Q�[�W��`�悷��
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

// ���݂�FPS��`�悷��
void HUD::DrawFrameRate(Shader* shader)
{
	int n = mGame->GetFrameRate();
	std::string s = "FPS:" + std::to_string(n);
	mFrameRate = mFont->RenderText(s, Color::Black, FPS_FONT_SIZE);
	DrawTexture(shader, mFrameRate, Vector2(FPS_POSITION));
}