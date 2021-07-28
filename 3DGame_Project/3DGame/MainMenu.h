#pragma once
#include "UIScreen.h"

#define MENU_START_BUTTON_POSITION Vector2(0, -50.0f)
#define MENU_QUIT_BUTTON_POSITION  Vector2(0, -130.0f)

// �v���g�^�C�v�錾
class Game;

class MainMenu : public UIScreen
{
public:
	MainMenu(Game* game);
	~MainMenu();

	void Draw(Shader* shader) override;

private:

	// �Q�[��Scene�����[�h����
	void  LoadGameScene();

	Texture* mTitle;
};