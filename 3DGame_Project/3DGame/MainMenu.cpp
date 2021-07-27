#include "MainMenu.h"
#include "Game.h"
#include "InputSystem.h"
#include "DialogBox.h"
#include "GameScene.h"

MainMenu::MainMenu(Game* game)
	:UIScreen(game)
{
	// ��|�[�Y���ł����͂��󂯎��悤�ɐݒ�
	mIsInputAccept = true;

	// �}�E�X�̑��΃��[�h��L���ɂ���
	mGame->GetInputSystem()->SetRelativeMouseMode(true);

	SetTitle("[ 3D GAME ]");
	AddButton("Start", [this]() {
		LoadGameScene();
	});
	AddButton("Quit", [this]() {
		new DialogBox(mGame, "Quit?", mIsInputAccept, [this]() {mGame->SetState(Game::GameState::EQuit); });
	});
}

MainMenu::~MainMenu()
{
	
}

void MainMenu::LoadGameScene()
{
	mGame->LoadScene(new GameScene(mGame));
}