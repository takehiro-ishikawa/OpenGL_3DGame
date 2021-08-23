#include "Menu.h"
#include "Game.h"
#include "Renderer.h"
#include "InputSystem.h"
#include "Scene.h"
#include "Audio.h"

MainMenu::MainMenu(Game* game)
	:UIScreen(game)
{
	// ��|�[�Y���ł����͂��󂯎��悤�ɐݒ�
	mIsInputAccept = true;

	// �}�E�X�̑��΃��[�h��L���ɂ���
	mGame->GetInputSystem()->SetRelativeMouseMode(true);

	mTitle = mGame->GetRenderer()->GetTexture("Assets/Textures/UI/Title.png");

	AddButton("Start", MAINMENU_STARTBUTTON_POSITION, [this]() {
		mGame->GetAudioSystem()->PlayEvent(SE_OK);
		LoadGameScene();
	});
	AddButton("Quit", MAINMENU_QUITBUTTON_POSITION, [this]() {
		mGame->GetAudioSystem()->PlayEvent(SE_OK);
		new DialogBox(mGame, "Quit?", mIsInputAccept, [this]() {mGame->SetState(Game::GameState::EQuit); });
	});
}

MainMenu::~MainMenu()
{
	
}

void MainMenu::Draw(Shader* shader)
{
	DrawTexture(shader, mTitle, Vector2(0, 100));

	UIScreen::Draw(shader);
}

void MainMenu::LoadGameScene()
{
	mGame->LoadScene(new GameScene(mGame));
}