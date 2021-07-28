#pragma once
#include "UIScreen.h"

#define MENU_START_BUTTON_POSITION Vector2(0, -50.0f)
#define MENU_QUIT_BUTTON_POSITION  Vector2(0, -130.0f)

// プロトタイプ宣言
class Game;

class MainMenu : public UIScreen
{
public:
	MainMenu(Game* game);
	~MainMenu();

	void Draw(Shader* shader) override;

private:

	// ゲームSceneをロードする
	void  LoadGameScene();

	Texture* mTitle;
};