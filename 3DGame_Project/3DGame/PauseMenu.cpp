#include "PauseMenu.h"
#include "Game.h"
#include "DialogBox.h"
#include <SDL/SDL.h>
#include "InputSystem.h"
#include "StartScene.h"

PauseMenu::PauseMenu(Game* game)
	:UIScreen(game)
{
	mGame->SetState(Game::GameState::EPaused);
	mGame->GetInputSystem()->SetRelativeMouseMode(false);
	SetTitle("Pause");
	AddButton("Resume", [this]() { Close(); });
	AddButton("Back to Menu", [this]() {
		new DialogBox(mGame, "Back to Menu ?", mIsInputAccept, [this]() {mGame->LoadScene(new StartScene(mGame));});
	});
}

PauseMenu::~PauseMenu()
{
	mGame->SetState(Game::GameState::EGameplay);
	mGame->GetInputSystem()->SetIsResumed(true);
	mGame->GetInputSystem()->SetRelativeMouseMode(true);
}

void PauseMenu::HandleKeyPress(const struct InputState& state)
{
	UIScreen::HandleKeyPress(state);

	if (state.Keyboard.GetKeyState(SDL_SCANCODE_ESCAPE) == ButtonState::EReleased)
	{
		Close();
	}
}
