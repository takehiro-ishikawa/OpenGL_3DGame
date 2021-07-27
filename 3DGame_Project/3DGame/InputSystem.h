#pragma once
#include <SDL/SDL_scancode.h>
#include <SDL/SDL_gamecontroller.h>
#include <SDL/SDL_mouse.h>
#include "Math.h"
#include <string>
#include <unordered_map>

// ���͖�
#define INPUT_LEFT_AXIS    "LeftAxis"
#define INPUT_RIGHT_AXIS   "RightAxis"
#define INPUT_JUMP         "Jump"
#define INPUT_UP           "Up"
#define INPUT_DOWN         "Down"
#define INPUT_SPRINT       "Sprint"
#define INPUT_FIRE         "Fire"
#define INPUT_FIRE_STANDBY "FireStandBy"

// �{�^���̓��͏��
enum class ButtonState
{
	ENone,     // �ǂ�ł��Ȃ�
	EPressed,  // ������
	EReleased, // ������
	EHeld      // �������܂�
};

// �L�[�{�[�h���͂̃w���p�[
class KeyboardState
{
public:
	// InputSystem��friend�N���X�Ȃ̂ŗe�ՂɍX�V�ł���
	friend class InputSystem;

	// �L�[�̐^�U�l�݂̂��擾����
	bool GetKeyValue(SDL_Scancode keyCode) const;

	// ���݂ƁA����1�O�̃t���[�������Ԃ��擾
	ButtonState GetKeyState(SDL_Scancode keyCode) const;

private:
	const Uint8* mCurrState;             // ���݂̓��͏��
	Uint8 mPrevState[SDL_NUM_SCANCODES]; // 1�O�̓��͏��
};

// �}�E�X���͂̃w���p�[
class MouseState
{
public:
	friend class InputSystem;

	// �}�E�X�|�W�V�����p
	const Vector2& GetPosition() const { return mMousePos; }
	const Vector2& GetScrollWheel() const { return mScrollWheel; }
	bool IsRelative() const { return mIsRelative; }

	// �}�E�X�{�^���p
	bool GetButtonValue(int button) const;
	ButtonState GetButtonState(int button) const;

private:
	// �}�E�X�̈ʒu���i�[
	Vector2 mMousePos;

	// �}�E�X�z�C�[���̓������i�[
	Vector2 mScrollWheel;

	// �{�^���̃f�[�^���i�[
	Uint32 mCurrButtons; // ���݂̓��͏��
	Uint32 mPrevButtons; // 1�O�̓��͏��

	// ���΃}�E�X���[�h���H
	bool mIsRelative;
};

// �R���g���[�����͂̃w���p�[
class ControllerState
{
public:
	friend class InputSystem;

	// �{�^���p
	bool GetButtonValue(SDL_GameControllerButton button) const;
	ButtonState GetButtonState(SDL_GameControllerButton button) const;

	const Vector2& GetLeftStick() const { return mLeftStick; }
	const Vector2& GetRightStick() const { return mRightStick; }
	float GetLeftTrigger() const { return mLeftTrigger; }
	float GetRightTrigger() const { return mRightTrigger; }

	bool GetIsConnected() const { return mIsConnected; }

private:
	// ����/�O��̃{�^��
	Uint8 mCurrButtons[SDL_CONTROLLER_BUTTON_MAX];
	Uint8 mPrevButtons[SDL_CONTROLLER_BUTTON_MAX];

	// ��/�E �A�i���O�X�e�B�b�N
	Vector2 mLeftStick;
	Vector2 mRightStick;

	// ��/�E �g���K�[
	float mLeftTrigger;
	float mRightTrigger;

	// ���̃R���g���[���͐ڑ�����Ă��邩?
	bool mIsConnected;
};

// �f�o�C�X���̓���̃{�^���萔���i�[���郉�b�p�[
struct InputCode
{
	SDL_Scancode keyBoard;                     // �L�[�{�[�h
	int mouseButton;                           // �}�E�X
	SDL_GameControllerButton controllerButton; // �R���g���[���[
};

// �f�o�C�X����Axis���͂��i�[���郉�b�p�[
struct AxisCode
{
	// �L�[�{�[�h
	SDL_Scancode vertical_High;   // �c���́{����
	SDL_Scancode vertical_Low;    // �c���́|����
	SDL_Scancode horizontal_High; // �����́{����
	SDL_Scancode horizontal_Low;  // �����́|����

	bool isMouseAxis;             // �}�E�X�̈ʒu���󂯎�邩

	bool isCtrlRightAxis;         // �R���g���[���[�̉E�ƍ��ǂ���̃X�e�B�b�N���͂��󂯎�邩
};

// ���͂��ꂽ�f�o�C�X
enum class InputDevice
{
	EMouse,      // �}�E�X
	EKeyBoard,   // �L�[�{�[�h
	EController, // �R���g���[��
	ENone        // �ǂ�ł��Ȃ�
};

// ���݂̓��͏�Ԃ��i�[���郉�b�p�[
struct InputState
{
	KeyboardState Keyboard;     // �L�[�{�[�h
	MouseState Mouse;           // �}�E�X
	ControllerState Controller; // �R���g���[���[

	// �A�N�V�����̖��O�ɑΉ�����f�o�C�X�̓��͏�Ԃ�Ԃ�
	ButtonState GetMappedButtonState(const std::string& actionName) const;
	Vector2 GetMappedAxis(const std::string& axisName, InputDevice& device) const;

	// mInputMaps�ւ̒l�̊i�[�Ɏg�p����w���p�[�֐�
	void SetInputMap(const std::string& actionName,
		SDL_Scancode keyBoard, int mouseButton, SDL_GameControllerButton controllerButton);
	// mAxisMaps�ւ̒l�̊i�[�Ɏg�p����w���p�[�֐�
	void SetAxisMap(const std::string& axisName, SDL_Scancode verHigh, SDL_Scancode verLow,
		SDL_Scancode horHigh, SDL_Scancode horLow, bool mouseValue, bool ctrlValue);
	
	std::unordered_map<std::string, InputCode*> mInputMaps;// �f�o�C�X���̃{�^���萔���i�[����A�z�z��
	std::unordered_map<std::string, AxisCode*> mAxisMaps;  // �f�o�C�X����Axis���͂��i�[����A�z�z��
};

// �S�Ă̓��͏������Ǘ�
class InputSystem
{
public:
	bool Initialize();
	void Shutdown();

	// SDL_PollEvents���[�v�̒��O�ɌĂ΂��
	void PrepareForUpdate();

	// SDL_PollEvents���[�v�̒���ɌĂ΂��
	void Update();

	// InputSystem��SDL�C�x���g���������邽�߂ɌĂяo�����
	void ProcessEvent(union SDL_Event& event);

	const InputState& GetState() const { return mState; }

	void SetRelativeMouseMode(bool value);

	void SetIsResumed(bool value) { mIsResumed = value; }

private:
	float Filter1D(int input);                // �g���K�[���͂̒l�Ƀt�B���^�[��������
	Vector2 Filter2D(int inputX, int inputY); // �A�i���O�X�e�B�b�N���͂̒l�Ƀt�B���^�[��������

	InputState mState;
	SDL_GameController* mController;

	// �|�[�Y�������������ォ�H
	bool mIsResumed;
};
