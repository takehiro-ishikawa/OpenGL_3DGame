#include "MainMenu.h"
#include "Game.h"
#include "Renderer.h"
#include "InputSystem.h"
#include "DialogBox.h"
#include "GameScene.h"

MainMenu::MainMenu(Game* game)
	:UIScreen(game)
{
	// 非ポーズ中でも入力を受け取るように設定
	mIsInputAccept = true;

	// マウスの相対モードを有効にする
	mGame->GetInputSystem()->SetRelativeMouseMode(true);

	mTitle = mGame->GetRenderer()->GetTexture("Assets/Textures/UI/Title.png");

	AddButton("Start", MENU_START_BUTTON_POSITION, [this]() {
		LoadGameScene();
	});
	AddButton("Quit", MENU_QUIT_BUTTON_POSITION, [this]() {
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