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

// �_�C�A���O
class DialogBox : public UIScreen
{
public:
	// �i�Ⴂ�h���[�I�[�_�[�́A����Ɍ�ɑΉ�����j
	DialogBox(Game* game, const std::string& text, bool isInputAccept, std::function<void()> onOK);
	~DialogBox();
};


#define MAINMENU_STARTBUTTON_POSITION Vector2(0, -50.0f)
#define MAINMENU_QUITBUTTON_POSITION  Vector2(0, -130.0f)

// �Q�[���N�����̃��C�����j���[���
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


#define PAUSE_TITLE_POSITION Vector2(0, 200.0f)   // �^�C�g���̈ʒu
#define PAUSE_RESUME_POSITION Vector2(0, 100.0f)  // Resume�{�^���̈ʒu
#define PAUSE_BACKMENU_POSITION Vector2(0, 30.0f) // BackMenu�{�^���̈ʒu

// �|�[�Y���
class PauseMenu : public UIScreen
{
public:
	PauseMenu(Game* game);
	~PauseMenu();

	void HandleKeyPress(const struct InputState& state) override;
	void Close() override;

	// ���C�����j���[��ʂɖ߂�
	void BackMenu();
};


#define RESULT_BG_FILEPATH       "Assets/Textures/UI/ResultBack.png"
#define RESULT_BG_POSITION       Vector2(0, 0)      // �w�i�摜�̈ʒu
#define RESULT_TITLE_POSITION    Vector2(0, 100.0f) // �^�C�g���̈ʒu
#define RESULT_BACKMENU_POSITION Vector2(0, -50.0f) // Resume�{�^���̈ʒu

// �Q�[���I�[�o�[���̉��
class ResultMenu : public UIScreen
{
public:
	ResultMenu(Game* mGame);
	~ResultMenu();

	// ���C�����j���[��ʂɖ߂�
	void BackMenu();
};
