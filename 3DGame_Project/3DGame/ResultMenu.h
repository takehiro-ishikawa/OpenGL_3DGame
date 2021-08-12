#pragma once
#include "UIScreen.h"

#define GAMEOVER_BG_FILEPATH       "Assets/Textures/UI/ResultBack.png"
#define GAMEOVER_BG_POSITION       Vector2(0, 0)      // 背景画像の位置
#define GAMEOVER_TITLE_POSITION    Vector2(0, 100.0f) // タイトルの位置
#define GAMEOVER_BACKMENU_POSITION Vector2(0, -50.0f) // Resumeボタンの位置

// プロトタイプ宣言
class Game;

class ResultMenu : public UIScreen
{
public:
	ResultMenu(Game* mGame);
	~ResultMenu();

	// メインメニュー画面に戻る
	void BackMenu();
};