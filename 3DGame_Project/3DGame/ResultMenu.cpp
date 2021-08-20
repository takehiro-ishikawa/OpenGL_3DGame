#include "ResultMenu.h"
#include "Game.h"
#include "Renderer.h"
#include "InputSystem.h"
#include "AudioSystem.h"
#include "StartScene.h"

ResultMenu::ResultMenu(Game* mGame)
	:UIScreen(mGame)
{
	mGame->SetState(Game::GameState::EPaused);
	mGame->GetInputSystem()->SetRelativeMouseMode(true);
	mTitlePos = GAMEOVER_TITLE_POSITION;

	// ”wŒi‚ÌƒeƒNƒXƒ`ƒƒ‚ðÝ’è
	mBGPos = GAMEOVER_BG_POSITION;
	mBackground = mGame->GetRenderer()->GetTexture(GAMEOVER_BG_FILEPATH);
	SetTitle("GAME OVER...", Color::Red);
	AddButton("Back to Menu", GAMEOVER_BACKMENU_POSITION, [this]() { BackMenu(); });
}

ResultMenu::~ResultMenu()
{
	mGame->SetState(Game::GameState::EGameplay);
	mGame->GetInputSystem()->SetIsResumed(true);
}

void ResultMenu::BackMenu()
{
	mGame->GetAudioSystem()->PlayEvent(SE_OK);

	mGame->LoadScene(new StartScene(mGame));
}