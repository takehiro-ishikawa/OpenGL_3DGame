#pragma once
#include "UIScreen.h"

// �v���g�^�C�v�錾
class Game;

class PauseMenu : public UIScreen
{
public:
	PauseMenu(Game* game);
	~PauseMenu();

	void HandleKeyPress(const struct InputState& state) override;
};