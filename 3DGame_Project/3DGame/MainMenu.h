#pragma once
#include "UIScreen.h"

// �v���g�^�C�v�錾
class Game;

class MainMenu : public UIScreen
{
public:
	MainMenu(Game* game);
	~MainMenu();

private:

	void  LoadGameScene();
};