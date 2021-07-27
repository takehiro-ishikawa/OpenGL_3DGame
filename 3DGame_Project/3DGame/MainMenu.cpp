#include "MainMenu.h"
#include "Game.h"
#include "InputSystem.h"
#include "DialogBox.h"
#include "GameScene.h"

MainMenu::MainMenu(Game* game)
	:UIScreen(game)
{
	// 非ポーズ中でも入力を受け取るように設定
	mIsInputAccept = true;

	// マウスの相対モードをOFFにしてカーソルを非表示
	SDL_ShowCursor(SDL_DISABLE);
	mGame->GetInputSystem()->SetRelativeMouseMode(false);

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