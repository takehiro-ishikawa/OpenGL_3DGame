#pragma once
#include "UIScreen.h"

#define GAMEOVER_BG_FILEPATH       "Assets/Textures/UI/ResultBack.png"
#define GAMEOVER_BG_POSITION       Vector2(0, 0)      // �w�i�摜�̈ʒu
#define GAMEOVER_TITLE_POSITION    Vector2(0, 100.0f) // �^�C�g���̈ʒu
#define GAMEOVER_BACKMENU_POSITION Vector2(0, -50.0f) // Resume�{�^���̈ʒu

// �v���g�^�C�v�錾
class Game;

class ResultMenu : public UIScreen
{
public:
	ResultMenu(Game* mGame);
	~ResultMenu();

	// ���C�����j���[��ʂɖ߂�
	void BackMenu();
};