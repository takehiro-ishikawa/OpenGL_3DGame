#pragma once
#include "Math.h"
#include <cstdint>
#include <string>
#include <functional>
#include <vector>

#define CURSOR_SPEED 500.0f

#pragma region プロトタイプ宣言
class Font;
class Texture;
class Game;
class Shader;
#pragma endregion

class Button
{
public:
	// コンストラクタは、ボタン名称と、フォントと、コールバック関数と、
	// ボタンの位置および寸法を受け取る
	Button(const std::string& name, Font* font,
		std::function<void()> onClick,
		const Vector2& pos, const Vector2& dims);
	~Button();

	// 表示テキストを設定し、テクスチャを生成する
	void SetName(const std::string& name);

	// 座標がボタンの範囲内ならtrueを返す
	bool ContainsPoint(const Vector2& pt) const;
	// ボタンが押された時に呼び出される
	void OnClick();

	// ゲッター/セッター
	Texture* GetNameTex() { return mNameTex; }
	const Vector2& GetPosition() const { return mPosition; }
	void SetHighlighted(bool sel) { mHighlighted = sel; }
	bool GetHighlighted() const { return mHighlighted; }
	
private:
	std::function<void()> mOnClick; // カプセル化したコールバック関数
	std::string mName;              // ボタンの名前
	Texture* mNameTex;              // ボタンのテクスチャ
	Font* mFont;                    // ボタンのフォント
	Vector2 mPosition;              // ボタンの位置
	Vector2 mDimensions;
	bool mHighlighted;
};


class UIScreen
{
public:
	UIScreen(Game* game);
	virtual ~UIScreen();

	// UIScreenサブクラスは以下をオーバーライドできる
	virtual void Update(float deltaTime);
	virtual void Draw(Shader* shader);
	virtual void ProcessInput(const struct InputState& state);
	virtual void HandleKeyPress(const struct InputState& state);

	// 状態がアクティブかクロージングかを管理
	enum UIState
	{
		EActive,
		EClosing
	};

	// 状態をクロージングにする
	void Close();

	// ゲッター
	UIState GetState() const { return mState; }
	bool GetIsInputAccept() { return mIsInputAccept; } 

	// タイトルの文字列を変更
	void SetTitle(const std::string& text, const Vector3& color = Color::White, int pointSize = 40);

	// ボタンを追加する
	void AddButton(const std::string& name, Vector2 pos, std::function<void()> onClick);

protected:
	// テクスチャを描画するヘルパー関数
	void DrawTexture(Shader* shader, Texture* texture, const Vector2& offset = Vector2::Zero, 
		Vector2 scale = Vector2::One);

	// マウスモードを相対かどうかに設定
	void SetRelativeMouseMode(bool relative);
	Game* mGame;

	// UI画面のタイトル文字列用
	Font* mFont;          // ボタンテキストのフォント
	Texture* mTitle;      // タイトルのテクスチャ

	// テクスチャ
	Texture* mBackground; // ボタン背景のテクスチャ
	Texture* mButtonOn;   // 選択されているボタンのテクスチャ
	Texture* mButtonOff;  // 選択されていないボタンのテクスチャ
	Texture* mCursor;     // カーソル

	// UIの各要素を構成する時の位置
	Vector2 mTitlePos;
	Vector2 mBGPos;

	// State
	UIState mState;
	// Buttonポインタ群の配列
	std::vector<Button*> mButtons;

	// 非ポーズ中に入力処理を受け付けるか？
	bool mIsInputAccept;

	// カーソルの移動量
	Vector2 mCursorMovePos;
	// マウスからの入力があったか？
	bool mIsInputMouse;
};
