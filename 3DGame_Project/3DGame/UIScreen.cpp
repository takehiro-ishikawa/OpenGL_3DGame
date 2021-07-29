#include "UIScreen.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Font.h"
#include "InputSystem.h"
#include "AudioSystem.h"

UIScreen::UIScreen(Game* game)
	:mGame(game)
	, mTitle(nullptr)
	, mBackground(nullptr)
	, mTitlePos(0.0f, 300.0f)
	, mBGPos(0.0f, 250.0f)
	, mIsInputAccept(false)
	, mCursorMovePos(Vector2::Zero)
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
	// ボタンがあるか?
	if (!mButtons.empty())
	{
		// カーソルの位置を取得
		Vector2 cursorPos = mGame->GetRenderer()->GetCursorPosition();
		// 移動量を加算する
		if (mIsInputMouse) cursorPos += mCursorMovePos;
		else cursorPos += mCursorMovePos * CURSOR_SPEED * deltaTime;
		// カーソルの位置を設定
		mGame->GetRenderer()->SetCursorPosition(cursorPos);
	}
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
		InputDevice device;
		Vector2 leftAxis = state.GetMappedAxis(INPUT_LEFT_AXIS, device);
		// コントローラかキーボードの入力があった場合
		if (device == InputDevice::EController || device == InputDevice::EKeyBoard)
		{
			// コントローラかキーボードからの入力値を移動量として設定
			mCursorMovePos = leftAxis;
			mIsInputMouse = false;
		}
		else
		{
			// マウスの相対座標を移動量として設定
			Vector2 mousePos = state.Mouse.GetPosition();
			mousePos.y *= -1; // Y成分を反転
			mCursorMovePos = mousePos;
			mIsInputMouse = true;
		}

		// 選択されているボタンの強調
		for (auto b : mButtons)
		{
			if (b->ContainsPoint(mGame->GetRenderer()->GetCursorPosition()))
			{
				if (!b->GetHighlighted()) mGame->GetAudioSystem()->PlayEvent(SE_SELECT);
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
	// 決定キーが押された時
	if (state.GetMappedButtonState(INPUT_OK) == ButtonState::EPressed)
	{
		// ボタンがあるか？
		if (!mButtons.empty())
		{
			// カーソルが範囲内に入っていたボタンのOnClick()を実行する
			for (auto b : mButtons)
			{
				if (b->GetHighlighted())
				{
					mCursorMovePos = Vector2::Zero;
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
	// 以前のタイトルテクスチャが存在する場合はクリアする
	if (mTitle)
	{
		mTitle->Unload();
		delete mTitle;
		mTitle = nullptr;
	}
	mTitle = mFont->RenderText(text, color, pointSize);
}

void UIScreen::AddButton(const std::string& name, Vector2 pos, std::function<void()> onClick)
{
	Vector2 dims(static_cast<float>(mButtonOn->GetWidth()),
		         static_cast<float>(mButtonOn->GetHeight()));
	Button* b = new Button(name, mFont, onClick, pos, dims);
	mButtons.emplace_back(b);
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