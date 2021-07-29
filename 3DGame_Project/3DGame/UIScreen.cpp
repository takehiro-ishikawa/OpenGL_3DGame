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
	// UI Stack�Ɏ��g��ǉ�
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
	// �{�^�������邩?
	if (!mButtons.empty())
	{
		// �J�[�\���̈ʒu���擾
		Vector2 cursorPos = mGame->GetRenderer()->GetCursorPosition();
		// �ړ��ʂ����Z����
		if (mIsInputMouse) cursorPos += mCursorMovePos;
		else cursorPos += mCursorMovePos * CURSOR_SPEED * deltaTime;
		// �J�[�\���̈ʒu��ݒ�
		mGame->GetRenderer()->SetCursorPosition(cursorPos);
	}
}

void UIScreen::Draw(Shader* shader)
{
	// �w�i��`��i���݂���ꍇ�j
	if (mBackground)
	{
		DrawTexture(shader, mBackground, mBGPos);
	}
	// �^�C�g����`��i���݂���ꍇ�j
	if (mTitle)
	{
		DrawTexture(shader, mTitle, mTitlePos);
	}
	// �{�^���̕`��
	for (auto b : mButtons)
	{
		// �{�^���̔w�i��`��
		Texture* tex = b->GetHighlighted() ? mButtonOn : mButtonOff;
		DrawTexture(shader, tex, b->GetPosition());
		// �{�^���̃e�L�X�g��`��
		DrawTexture(shader, b->GetNameTex(), b->GetPosition());
	}
	// �J�[�\���̕`��
	if (mCursor)
	{
		DrawTexture(shader, mCursor, mGame->GetRenderer()->GetCursorPosition());
	}
	// �T�u�N���X�ŃI�[�o�[���C�h���āA�e�N�X�`����`�悷��
}

void UIScreen::ProcessInput(const struct InputState& state)
{
	// �{�^�������邩?
	if (!mButtons.empty())
	{
		InputDevice device;
		Vector2 leftAxis = state.GetMappedAxis(INPUT_LEFT_AXIS, device);
		// �R���g���[�����L�[�{�[�h�̓��͂��������ꍇ
		if (device == InputDevice::EController || device == InputDevice::EKeyBoard)
		{
			// �R���g���[�����L�[�{�[�h����̓��͒l���ړ��ʂƂ��Đݒ�
			mCursorMovePos = leftAxis;
			mIsInputMouse = false;
		}
		else
		{
			// �}�E�X�̑��΍��W���ړ��ʂƂ��Đݒ�
			Vector2 mousePos = state.Mouse.GetPosition();
			mousePos.y *= -1; // Y�����𔽓]
			mCursorMovePos = mousePos;
			mIsInputMouse = true;
		}

		// �I������Ă���{�^���̋���
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
	// ����L�[�������ꂽ��
	if (state.GetMappedButtonState(INPUT_OK) == ButtonState::EPressed)
	{
		// �{�^�������邩�H
		if (!mButtons.empty())
		{
			// �J�[�\�����͈͓��ɓ����Ă����{�^����OnClick()�����s����
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
	// �ȑO�̃^�C�g���e�N�X�`�������݂���ꍇ�̓N���A����
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
	// ��`���e�N�X�`���̕�/�����ŃX�P�[�����O����
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(texture->GetWidth()) * scale.x,
		static_cast<float>(texture->GetHeight()) * scale.y,
		1.0f);
	// ��ʏ�̈ʒu�ɕϊ�
	Matrix4 transMat = Matrix4::CreateTranslation(
		Vector3(offset.x, offset.y, 0.0f));
	// ���[���h��Ԃɕϊ�
	Matrix4 world = scaleMat * transMat;
	shader->SetMatrixUniform("uWorldTransform", world);
	// ���݂̃e�N�X�`����ݒ�
	texture->SetActive();
	// ��`��`��
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void UIScreen::SetRelativeMouseMode(bool relative)
{
	if (relative)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		// �N���A���邽�߂ɑ��ΓI�Ɏ擾���邽�߂̍ŏ��̌Ăяo�����s��
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
	// �A�^�b�`���ꂽ�n���h���[�����݂���ꍇ�́A������Ăяo��
	if (mOnClick)
	{
		mOnClick();
	}
}