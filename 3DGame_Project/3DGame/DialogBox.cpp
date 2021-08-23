#include "Menu.h"
#include "Game.h"
#include "Audio.h"
#include "Renderer.h"
#include <SDL/SDL.h>

DialogBox::DialogBox(Game* game, const std::string& text, bool isInputAccept, std::function<void()> onOK)
	:UIScreen(game)
{
	mIsInputAccept = isInputAccept;

	// ダイアログボックス用に位置を調整
	mBGPos = DIALOG_BG_POSITION;
	mTitlePos = DIALOG_TITLE_POSITION;

	// 背景のテクスチャを設定
	mBackground = mGame->GetRenderer()->GetTexture(DIALOG_BG_FILEPATH);
	SetTitle(text, Vector3::Zero, DIALOG_TITLE_SIZE);

	// ボタンの設定
	AddButton("OK", DIALOG_OK_BUTTON_POS, [onOK]() {
		onOK();
	});
	AddButton("Cancel", DIALOG_CANCEL_BUTTON_POS, [this]() {
		mGame->GetAudioSystem()->PlayEvent(SE_CANCEL);
		Close();
	});
}

DialogBox::~DialogBox()
{
}
