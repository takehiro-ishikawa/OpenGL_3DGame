#include "UIScreen.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Font.h"
#include "InputSystem.h"

UIScreen::UIScreen(Game* game)
	:mGame(game)
	, mTitle(nullptr)
	, mBackground(nullptr)
	, mTitlePos(0.0f, 300.0f)
	, mNextButtonPos(0.0f, 200.0f)
	, mBGPos(0.0f, 250.0f)
	, mIsInputAccept(false)
	, mState(EActive)
{
	// UI Stackに自身を追加
	mGame->PushUI(this);

	mFont = mGame->GetFont("Assets/Carlito-Regular.ttf");
	mButtonOn = mGame->GetRenderer()->GetTexture("Assets/Textures/UI/ButtonYellow.png");
	mButtonOff = mGame->GetRenderer()->GetTexture("Assets/Textures/UI/ButtonBlue.png");
	mCursor = mGame->GetRenderer()->GetTexture("Assets/Textures/UI/Cursor.png");
}

UIScreen::~UIScreen()
{
	if (mTitle)
	{
		mTitle->Unload();
		delete mTitle;
	}

	for (auto b : mButtons)
	{
		delete b;
	}
	mButtons.clear();
}

void UIScreen::Update(float deltaTime)
{

}

void UIScreen::Draw(Shader* shader)
{
	// 背景を描画（存在する場合）
	if (mBackground)
	{
		DrawTexture(shader, mBackground, mBGPos);
	}
	// タイトルを描画（存在する場合）
	if (mTitle)
	{
		DrawTexture(shader, mTitle, mTitlePos);
	}
	// ボタンの描画
	for (auto b : mButtons)
	{
		// ボタンの背景を描画
		Texture* tex = b->GetHighlighted() ? mButtonOn : mButtonOff;
		DrawTexture(shader, tex, b->GetPosition());
		// ボタンのテキストを描画
		DrawTexture(shader, b->GetNameTex(), b->GetPosition());
	}
	// カーソルの描画
	if (mCursor)
	{
		DrawTexture(shader, mCursor, mGame->GetRenderer()->GetCursorPosition());
	}
	// サブクラスでオーバーライドして、テクスチャを描画する
}

void UIScreen::ProcessInput(const struct InputState& state)
{
	// ボタンがあるか?
	if (!mButtons.empty())
	{
		Vector2 cursorPos = mGame->GetRenderer()->GetCursorPosition();

		// マウスの位置を取得
		Vector2 mousePos = state.Mouse.GetPosition();
		mousePos = mousePos - mPrevMousePos;
		mPrevMousePos = state.Mouse.GetPosition();

		InputDevice device;
		Vector2 leftAxis = state.GetMappedAxis(INPUT_LEFT_AXIS, device);
		// コントローラかキーボードの操作があった
		if (device == InputDevice::EController || device == InputDevice::EKeyBoard)
		{
			cursorPos += leftAxis * 8.0f;
		}
		else
		{
			
			cursorPos += mousePos;
		}
		// カーソルの位置を設定
		mGame->GetRenderer()->SetCursorPosition(cursorPos);

		// 選択されているボタンの強調
		for (auto b : mButtons)
		{
			if (b->ContainsPoint(cursorPos))
			{
				b->SetHighlighted(true);
			}
			else
			{
				b->SetHighlighted(false);
			}
		}
	}
}

void UIScreen::HandleKeyPress(const struct InputState& state)
{
	// マウスが左クリックされた時
	if (state.Mouse.GetButtonState(SDL_BUTTON_LEFT) == ButtonState::EPressed)
	{
		if (!mButtons.empty())
		{
			for (auto b : mButtons)
			{
				if (b->GetHighlighted())
				{
					b->OnClick();
					break;
				}
			}
		}
	}
}

void UIScreen::Close()
{
	mState = EClosing;
}

void UIScreen::SetTitle(const std::string& text,
	const Vector3& color,
	int pointSize)
{
	// Clear out previous title texture if it exists
	if (mTitle)
	{
		mTitle->Unload();
		delete mTitle;
		mTitle = nullptr;
	}
	mTitle = mFont->RenderText(text, color, pointSize);
}

void UIScreen::AddButton(const std::string& name, std::function<void()> onClick)
{
	Vector2 dims(static_cast<float>(mButtonOn->GetWidth()),
		         static_cast<float>(mButtonOn->GetHeight()));
	Button* b = new Button(name, mFont, onClick, mNextButtonPos, dims);
	mButtons.emplace_back(b);

	// 次のボタンの位置を更新
	// ボタンの高さ + 余白の分だけ位置を下げる
	mNextButtonPos.y -= mButtonOff->GetHeight() + 20.0f;
}

void UIScreen::DrawTexture(class Shader* shader, class Texture* texture,
	const Vector2& offset, Vector2 scale)
{
	// 矩形をテクスチャの幅/高さでスケーリングする
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(texture->GetWidth()) * scale.x,
		static_cast<float>(texture->GetHeight()) * scale.y,
		1.0f);
	// 画面上の位置に変換
	Matrix4 transMat = Matrix4::CreateTranslation(
		Vector3(offset.x, offset.y, 0.0f));
	// ワールド空間に変換
	Matrix4 world = scaleMat * transMat;
	shader->SetMatrixUniform("uWorldTransform", world);
	// 現在のテクスチャを設定
	texture->SetActive();
	// 矩形を描画
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void UIScreen::SetRelativeMouseMode(bool relative)
{
	if (relative)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		// クリアするために相対的に取得するための最初の呼び出しを行う
		SDL_GetRelativeMouseState(nullptr, nullptr);
	}
	else
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}

Button::Button(const std::string& name, Font* font,
	std::function<void()> onClick,
	const Vector2& pos, const Vector2& dims)
	:mOnClick(onClick)
	, mNameTex(nullptr)
	, mFont(font)
	, mPosition(pos)
	, mDimensions(dims)
	, mHighlighted(false)
{
	SetName(name);
}

Button::~Button()
{
	if (mNameTex)
	{
		mNameTex->Unload();
		delete mNameTex;
	}
}

void Button::SetName(const std::string& name)
{
	mName = name;

	if (mNameTex)
	{
		mNameTex->Unload();
		delete mNameTex;
		mNameTex = nullptr;
	}
	mNameTex = mFont->RenderText(mName);
}

bool Button::ContainsPoint(const Vector2& pt) const
{
	bool no = pt.x < (mPosition.x - mDimensions.x / 2.0f) ||
		pt.x >(mPosition.x + mDimensions.x / 2.0f) ||
		pt.y < (mPosition.y - mDimensions.y / 2.0f) ||
		pt.y >(mPosition.y + mDimensions.y / 2.0f);
	return !no;
}

void Button::OnClick()
{
	// アタッチされたハンドラーが存在する場合は、それを呼び出す
	if (mOnClick)
	{
		mOnClick();
	}
}