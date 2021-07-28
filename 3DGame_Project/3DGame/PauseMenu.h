#pragma once
#include "UIScreen.h"

#define PAUSE_TITLE_POSITION Vector2(0, 200.0f)   // �^�C�g���̈ʒu
#define PAUSE_RESUME_POSITION Vector2(0, 100.0f)  // Resume�{�^���̈ʒu
#define PAUSE_BACKMENU_POSITION Vector2(0, 30.0f) // BackMenu�{�^���̈ʒu

// �v���g�^�C�v�錾
class Game;

class PauseMenu : public UIScreen
{
public:
	PauseMenu(Game* game);
	~PauseMenu();

	void HandleKeyPress(const struct InputState& state) override;
};