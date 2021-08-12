#pragma once
#include "UIScreen.h"

#define DIALOG_BG_FILEPATH "Assets/Textures/UI/DialogBG.png" // �w�i�摜�̃t�@�C���p�X
#define DIALOG_BG_POSITION       Vector2(0, 0)               // �w�i�摜�̈ʒu
#define DIALOG_TITLE_POSITION    Vector2(0, 100.0f)          // �^�C�g���̈ʒu
#define DIALOG_TITLE_SIZE        30                          // �^�C�g���̕����T�C�Y
#define DIALOG_OK_BUTTON_POS     Vector2(0, 0)               // OK�{�^���̈ʒu
#define DIALOG_CANCEL_BUTTON_POS Vector2(0, -80.0f)          // �L�����Z���{�^���̈ʒu

// �v���g�^�C�v�錾
class Game;

class DialogBox : public UIScreen
{
public:
	// �i�Ⴂ�h���[�I�[�_�[�́A����Ɍ�ɑΉ�����j
	DialogBox(Game* game, const std::string& text, bool isInputAccept, std::function<void()> onOK);
	~DialogBox();
};
