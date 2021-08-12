#pragma once
#include "UIScreen.h"

#define DIALOG_BG_FILEPATH "Assets/Textures/UI/DialogBG.png" // 背景画像のファイルパス
#define DIALOG_BG_POSITION       Vector2(0, 0)               // 背景画像の位置
#define DIALOG_TITLE_POSITION    Vector2(0, 100.0f)          // タイトルの位置
#define DIALOG_TITLE_SIZE        30                          // タイトルの文字サイズ
#define DIALOG_OK_BUTTON_POS     Vector2(0, 0)               // OKボタンの位置
#define DIALOG_CANCEL_BUTTON_POS Vector2(0, -80.0f)          // キャンセルボタンの位置

// プロトタイプ宣言
class Game;

class DialogBox : public UIScreen
{
public:
	// （低いドローオーダーは、さらに後に対応する）
	DialogBox(Game* game, const std::string& text, bool isInputAccept, std::function<void()> onOK);
	~DialogBox();
};
