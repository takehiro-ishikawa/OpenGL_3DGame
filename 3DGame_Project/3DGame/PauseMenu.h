#pragma once
#include "UIScreen.h"

#define PAUSE_TITLE_POSITION Vector2(0, 200.0f)   // タイトルの位置
#define PAUSE_RESUME_POSITION Vector2(0, 100.0f)  // Resumeボタンの位置
#define PAUSE_BACKMENU_POSITION Vector2(0, 30.0f) // BackMenuボタンの位置

// プロトタイプ宣言
class Game;

class PauseMenu : public UIScreen
{
public:
	PauseMenu(Game* game);
	~PauseMenu();

	void HandleKeyPress(const struct InputState& state) override;
};