#include "MainMenu.h"
#include "Game.h"
#include "InputSystem.h"
#include "DialogBox.h"
#include "GameScene.h"

MainMenu::MainMenu(Game* game)
	:UIScreen(game)
{
	mGame->SetState(Game::GameState::EPaused);
	mGame->GetInputSystem()->SetRelativeMouseMode(false);
	SetTitle("[ 3D GAME ]");
	AddButton("Start", [this]() {
		LoadGameScene();
	});
	AddButton("Quit", [this]() {
		new DialogBox(mGame, "Quit?", [this]() {mGame->SetState(Game::GameState::EQuit); });
	});
}

MainMenu::~MainMenu()
{
	
}

void MainMenu::LoadGameScene()
{
	mGame->LoadScene(new GameScene(mGame));
}