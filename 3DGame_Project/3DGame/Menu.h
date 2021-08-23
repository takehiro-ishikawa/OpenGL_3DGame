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

// ダイアログ
class DialogBox : public UIScreen
{
public:
	// （低いドローオーダーは、さらに後に対応する）
	DialogBox(Game* game, const std::string& text, bool isInputAccept, std::function<void()> onOK);
	~DialogBox();
};


#define MAINMENU_STARTBUTTON_POSITION Vector2(0, -50.0f)
#define MAINMENU_QUITBUTTON_POSITION  Vector2(0, -130.0f)

// ゲーム起動時のメインメニュー画面
class MainMenu : public UIScreen
{
public:
	MainMenu(Game* game);
	~MainMenu();

	void Draw(Shader* shader) override;

private:

	// ゲームSceneをロードする
	void  LoadGameScene();

	Texture* mTitle;
};


#define PAUSE_TITLE_POSITION Vector2(0, 200.0f)   // タイトルの位置
#define PAUSE_RESUME_POSITION Vector2(0, 100.0f)  // Resumeボタンの位置
#define PAUSE_BACKMENU_POSITION Vector2(0, 30.0f) // BackMenuボタンの位置

// ポーズ画面
class PauseMenu : public UIScreen
{
public:
	PauseMenu(Game* game);
	~PauseMenu();

	void HandleKeyPress(const struct InputState& state) override;
	void Close() override;

	// メインメニュー画面に戻る
	void BackMenu();
};


#define RESULT_BG_FILEPATH       "Assets/Textures/UI/ResultBack.png"
#define RESULT_BG_POSITION       Vector2(0, 0)      // 背景画像の位置
#define RESULT_TITLE_POSITION    Vector2(0, 100.0f) // タイトルの位置
#define RESULT_BACKMENU_POSITION Vector2(0, -50.0f) // Resumeボタンの位置

// ゲームオーバー時の画面
class ResultMenu : public UIScreen
{
public:
	ResultMenu(Game* mGame);
	~ResultMenu();

	// メインメニュー画面に戻る
	void BackMenu();
};
