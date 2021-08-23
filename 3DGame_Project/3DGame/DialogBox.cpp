#include "Menu.h"
#include "Game.h"
#include "Audio.h"
#include "Renderer.h"
#include <SDL/SDL.h>

DialogBox::DialogBox(Game* game, const std::string& text, bool isInputAccept, std::function<void()> onOK)
	:UIScreen(game)
{
	mIsInputAccept = isInputAccept;

	// �_�C�A���O�{�b�N�X�p�Ɉʒu�𒲐�
	mBGPos = DIALOG_BG_POSITION;
	mTitlePos = DIALOG_TITLE_POSITION;

	// �w�i�̃e�N�X�`����ݒ�
	mBackground = mGame->GetRenderer()->GetTexture(DIALOG_BG_FILEPATH);
	SetTitle(text, Vector3::Zero, DIALOG_TITLE_SIZE);

	// �{�^���̐ݒ�
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
