#pragma once
#include "UIScreen.h"

// プロトタイプ宣言
class Game;

class PauseMenu : public UIScreen
{
public:
	PauseMenu(Game* game);
	~PauseMenu();

	void HandleKeyPress(const struct InputState& state) override;
};