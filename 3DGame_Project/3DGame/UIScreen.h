#pragma once
#include "Math.h"
#include <cstdint>
#include <string>
#include <functional>
#include <vector>

#define CURSOR_SPEED 500.0f

#pragma region �v���g�^�C�v�錾
class Font;
class Texture;
class Game;
class Shader;
#pragma endregion

class Button
{
public:
	// �R���X�g���N�^�́A�{�^�����̂ƁA�t�H���g�ƁA�R�[���o�b�N�֐��ƁA
	// �{�^���̈ʒu����ѐ��@���󂯎��
	Button(const std::string& name, Font* font,
		std::function<void()> onClick,
		const Vector2& pos, const Vector2& dims);
	~Button();

	// �\���e�L�X�g��ݒ肵�A�e�N�X�`���𐶐�����
	void SetName(const std::string& name);

	// ���W���{�^���͈͓̔��Ȃ�true��Ԃ�
	bool ContainsPoint(const Vector2& pt) const;
	// �{�^���������ꂽ���ɌĂяo�����
	void OnClick();

	// �Q�b�^�[/�Z�b�^�[
	Texture* GetNameTex() { return mNameTex; }
	const Vector2& GetPosition() const { return mPosition; }
	void SetHighlighted(bool sel) { mHighlighted = sel; }
	bool GetHighlighted() const { return mHighlighted; }
	
private:
	std::function<void()> mOnClick; // �J�v�Z���������R�[���o�b�N�֐�
	std::string mName;              // �{�^���̖��O
	Texture* mNameTex;              // �{�^���̃e�N�X�`��
	Font* mFont;                    // �{�^���̃t�H���g
	Vector2 mPosition;              // �{�^���̈ʒu
	Vector2 mDimensions;
	bool mHighlighted;
};


class UIScreen
{
public:
	UIScreen(Game* game);
	virtual ~UIScreen();

	// UIScreen�T�u�N���X�͈ȉ����I�[�o�[���C�h�ł���
	virtual void Update(float deltaTime);
	virtual void Draw(Shader* shader);
	virtual void ProcessInput(const struct InputState& state);
	virtual void HandleKeyPress(const struct InputState& state);

	// ��Ԃ��A�N�e�B�u���N���[�W���O�����Ǘ�
	enum UIState
	{
		EActive,
		EClosing
	};

	// ��Ԃ��N���[�W���O�ɂ���
	void Close();

	// �Q�b�^�[
	UIState GetState() const { return mState; }
	bool GetIsInputAccept() { return mIsInputAccept; } 

	// �^�C�g���̕������ύX
	void SetTitle(const std::string& text, const Vector3& color = Color::White, int pointSize = 40);

	// �{�^����ǉ�����
	void AddButton(const std::string& name, Vector2 pos, std::function<void()> onClick);

protected:
	// �e�N�X�`����`�悷��w���p�[�֐�
	void DrawTexture(Shader* shader, Texture* texture, const Vector2& offset = Vector2::Zero, 
		Vector2 scale = Vector2::One);

	// �}�E�X���[�h�𑊑΂��ǂ����ɐݒ�
	void SetRelativeMouseMode(bool relative);
	Game* mGame;

	// UI��ʂ̃^�C�g��������p
	Font* mFont;          // �{�^���e�L�X�g�̃t�H���g
	Texture* mTitle;      // �^�C�g���̃e�N�X�`��

	// �e�N�X�`��
	Texture* mBackground; // �{�^���w�i�̃e�N�X�`��
	Texture* mButtonOn;   // �I������Ă���{�^���̃e�N�X�`��
	Texture* mButtonOff;  // �I������Ă��Ȃ��{�^���̃e�N�X�`��
	Texture* mCursor;     // �J�[�\��

	// UI�̊e�v�f���\�����鎞�̈ʒu
	Vector2 mTitlePos;
	Vector2 mBGPos;

	// State
	UIState mState;
	// Button�|�C���^�Q�̔z��
	std::vector<Button*> mButtons;

	// ��|�[�Y���ɓ��͏������󂯕t���邩�H
	bool mIsInputAccept;

	// �J�[�\���̈ړ���
	Vector2 mCursorMovePos;
	// �}�E�X����̓��͂����������H
	bool mIsInputMouse;
};
