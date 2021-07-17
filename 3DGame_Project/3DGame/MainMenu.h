#pragma once
#include "UIScreen.h"

// プロトタイプ宣言
class Game;

class MainMenu : public UIScreen
{
public:
	MainMenu(Game* game);
	~MainMenu();

private:

	void  LoadGameScene();
};