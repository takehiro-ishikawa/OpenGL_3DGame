// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "InputSystem.h"
#include <SDL.h>
#include <cstring>
#include <iostream>

bool KeyboardState::GetKeyValue(SDL_Scancode keyCode) const
{
	return mCurrState[keyCode] == 1;
}

ButtonState KeyboardState::GetKeyState(SDL_Scancode keyCode) const
{
	if (mPrevState[keyCode] == 0)
	{
		if (mCurrState[keyCode] == 0)
		{
			// �L�[�͌��݂��O�̃t���[���ł�������Ă��Ȃ�
			return ButtonState::ENone;
		}
		else
		{
			// �L�[�͌��݂̃t���[���ŉ�����
			return ButtonState::EPressed;
		}
	}
	else // mPrevState[keyCode]�͂P�ɈႢ�Ȃ�
	{
		if (mCurrState[keyCode] == 0)
		{
			// �L�[�͌��݂̃t���[���ŕ�����
			return ButtonState::EReleased;
		}
		else
		{
			// �L�[�͉����ꂽ�܂�
			return ButtonState::EHeld;
		}
	}
}

bool MouseState::GetButtonValue(int button) const
{
	return (SDL_BUTTON(button) & mCurrButtons) == 1;
}

ButtonState MouseState::GetButtonState(int button) const
{
	int mask = SDL_BUTTON(button);
	if ((mask & mPrevButtons) == 0)
	{
		if ((mask & mCurrButtons) == 0)
		{
			return ButtonState::ENone;
		}
		else
		{
			return ButtonState::EPressed;
		}
	}
	else
	{
		if ((mask & mCurrButtons) == 0)
		{
			return ButtonState::EReleased;
		}
		else
		{
			return ButtonState::EHeld;
		}
	}
}

bool ControllerState::GetButtonValue(SDL_GameControllerButton button) const
{
	return mCurrButtons[button] == 1;
}

ButtonState ControllerState::GetButtonState(SDL_GameControllerButton button) const
{
	if (mPrevButtons[button] == 0)
	{
		if (mCurrButtons[button] == 0)
		{
			return ButtonState::ENone;
		}
		else
		{
			return ButtonState::EPressed;
		}
	}
	else // Prev state must be 1
	{
		if (mCurrButtons[button] == 0)
		{
			return ButtonState::EReleased;
		}
		else
		{
			return ButtonState::EHeld;
		}
	}
}

ButtonState InputState::GetMappedButtonState(const std::string& actionName) const
{
	auto iter = mInputMaps.find(actionName);

	// actionName���A�z�z����ɖ����Ȃ�Enone��Ԃ�
	if (iter == mInputMaps.end()) return ButtonState::ENone;
	
	InputCode* code = iter->second;

	// ���ꂼ��̃f�o�C�X����̓��͏�Ԃ��󂯎��
	ButtonState keyState = Keyboard.GetKeyState(code->keyBoard);                     // �L�[�{�[�h	
	ButtonState mouseState = Mouse.GetButtonState(code->mouseButton);                // �}�E�X
	ButtonState controllerState = Controller.GetButtonState(code->controllerButton); // �R���g���[���[

	// ENone�łȂ����̂�D�悵�ĕԂ�
	ButtonState returnState = (keyState != ButtonState::ENone) ? keyState : ButtonState::ENone;
	if (mouseState != ButtonState::ENone) returnState = mouseState;
	if (controllerState != ButtonState::ENone) returnState = controllerState;

	return returnState;
}

Vector2 InputState::GetMappedAxis(const std::string& axisName, InputDevice& device) const
{
	auto iter = mAxisMaps.find(axisName);
	device = InputDevice::ENone;

	// actionName���A�z�z����ɖ����Ȃ�0��Ԃ�
	if (iter == mAxisMaps.end()) return Vector2::Zero;

	AxisCode* code = iter->second;
	Vector2 returnAxis = Vector2::Zero;

	// ���ꂼ��̃f�o�C�X����̓��͏�Ԃ��󂯎��
	// �}�E�X
	Vector2 inputAxis;
	if (code->isMouseAxis)
	{
		inputAxis = Mouse.GetPosition();

		if (inputAxis != Vector2::Zero)
		{
			device = InputDevice::EMouse;
			returnAxis = inputAxis;
		}
	}
	
	// �L�[�{�[�h
	inputAxis = Vector2::Zero;
	inputAxis.y += Keyboard.GetKeyValue(code->vertical_High) ? 1 : 0;
	inputAxis.y -= Keyboard.GetKeyValue(code->vertical_Low) ? 1 : 0;
	inputAxis.x += Keyboard.GetKeyValue(code->horizontal_High) ? 1 : 0;
	inputAxis.x -= Keyboard.GetKeyValue(code->horizontal_Low) ? 1 : 0;
	if (inputAxis != Vector2::Zero)
	{
		device = InputDevice::EKeyBoard;
		returnAxis = inputAxis;
	}

	// �R���g���[���[
	// �E�X�e�B�b�N
	if (code->isCtrlRightAxis)
	{
		inputAxis = Controller.GetRightStick();
		inputAxis.y *= -1;
	}
	// ���X�e�B�b�N
	else
	{
		inputAxis = Controller.GetLeftStick();
	}
	if (inputAxis != Vector2::Zero)
	{
		device = InputDevice::EController;
		returnAxis = inputAxis;
	}

	// ���͂����f�o�C�X���L�[�{�[�h���R���g���[���Ȃ�l�𐳋K������
	if (device == InputDevice::EKeyBoard || device == InputDevice::EController) returnAxis.Normalize();

	return returnAxis;
}

void InputState::SetInputMap(const std::string& actionName,
	SDL_Scancode keyBoard, int mouseButton, SDL_GameControllerButton controllerButton)
{
	InputCode* code = new InputCode;

	code->keyBoard = keyBoard;
	code->mouseButton = mouseButton;
	code->controllerButton = controllerButton;

	mInputMaps.emplace(actionName, code);
}

void  InputState::SetAxisMap(const std::string& axisName, SDL_Scancode verHigh, SDL_Scancode verLow,
	SDL_Scancode horHigh, SDL_Scancode horLow, bool mouseValue, bool ctrlValue)
{
	AxisCode* code = new AxisCode;

	code->vertical_High = verHigh;
	code->vertical_Low = verLow;
	code->horizontal_High = horHigh;
	code->horizontal_Low = horLow;
	code->isMouseAxis = mouseValue;
	code->isCtrlRightAxis = ctrlValue;

	mAxisMaps.emplace(axisName, code);
}

bool InputSystem::Initialize()
{
	// �L�[�{�[�h
	// ���݂̓��͏�Ԃ������|�C���^��ۑ�
	mState.Keyboard.mCurrState = SDL_GetKeyboardState(NULL);

	// �O��̏�Ԃ��������������N���A
	memset(mState.Keyboard.mPrevState, 0, SDL_NUM_SCANCODES);

	// �}�E�X�i���ׂĂ�0�ɐݒ肷�邾���j
	mState.Mouse.mCurrButtons = 0;
	mState.Mouse.mPrevButtons = 0;
	mState.Mouse.mIsRelative = false;

	// �R���g���[��
	// �ڑ�����Ă���R���g���[���[�����݂���ꍇ�́A������擾����
	mController = SDL_GameControllerOpen(0);

	// ControllerState�̏�����
	mState.Controller.mIsConnected = (mController != nullptr);
	memset(mState.Controller.mCurrButtons, 0,
		SDL_CONTROLLER_BUTTON_MAX);
	memset(mState.Controller.mPrevButtons, 0,
		SDL_CONTROLLER_BUTTON_MAX);

	// ���̓A�N�V�������Ƃ���ɑΉ�����f�o�C�X���̃{�^���萔�̒�`
	mState.SetAxisMap("LeftAxis", SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_A,
		false, false);
	mState.SetAxisMap("RightAxis", SDL_SCANCODE_DOWN, SDL_SCANCODE_UP, SDL_SCANCODE_RIGHT,
		SDL_SCANCODE_LEFT, true, true);
	mState.SetInputMap("Jump", SDL_SCANCODE_SPACE, 0, SDL_CONTROLLER_BUTTON_A);
	mState.SetInputMap("Up", SDL_SCANCODE_E, 0, SDL_CONTROLLER_BUTTON_B);
	mState.SetInputMap("Down", SDL_SCANCODE_Q, 0, SDL_CONTROLLER_BUTTON_X);

	mIsResumed = false;

	return true;
}

void InputSystem::Shutdown()
{
}

void InputSystem::PrepareForUpdate()
{
	// ���݂̏�Ԃ�O�̏�ԂɃR�s�[����
	// �L�[�{�[�h
	memcpy(mState.Keyboard.mPrevState,
		mState.Keyboard.mCurrState,
		SDL_NUM_SCANCODES);

	// �}�E�X
	mState.Mouse.mPrevButtons = mState.Mouse.mCurrButtons;
	mState.Mouse.mScrollWheel = Vector2::Zero;

	// �R���g���[��
	memcpy(mState.Controller.mPrevButtons,
		mState.Controller.mCurrButtons,
		SDL_CONTROLLER_BUTTON_MAX);
}

void InputSystem::Update()
{
	// �}�E�X�̓��͂𓾂�
	int x = 0, y = 0;
	
	// ���΃��[�hON
	if (mState.Mouse.mIsRelative)
	{
		mState.Mouse.mCurrButtons = 
			SDL_GetRelativeMouseState(&x, &y);
	}
	// ���΃��[�hOFF
	else
	{
		mState.Mouse.mCurrButtons = 
			SDL_GetMouseState(&x, &y);
	}

	// �|�[�Y��������������Ȃ�}�E�X�̓��͒l��0�ɂ���(Resume���ɃJ�����̊p�x���ς���Ă��܂��̂�)
	if (mIsResumed)
	{
		x = 0; y = 0;
		mIsResumed = false;
		SDL_Log("IsResumed");
	}

	mState.Mouse.mMousePos.x = static_cast<float>(x);
	mState.Mouse.mMousePos.y = static_cast<float>(y);

	// �R���g���[���̓��͂𓾂�
	// �{�^��
	for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
	{
		mState.Controller.mCurrButtons[i] =
			SDL_GameControllerGetButton(mController,
				SDL_GameControllerButton(i));
	}

	// �g���K�[
	mState.Controller.mLeftTrigger =
		Filter1D(SDL_GameControllerGetAxis(mController,
			SDL_CONTROLLER_AXIS_TRIGGERLEFT));
	mState.Controller.mRightTrigger =
		Filter1D(SDL_GameControllerGetAxis(mController,
			SDL_CONTROLLER_AXIS_TRIGGERRIGHT));

	// �X�e�B�b�N
	x = SDL_GameControllerGetAxis(mController,
		SDL_CONTROLLER_AXIS_LEFTX);
	y = -SDL_GameControllerGetAxis(mController,
		SDL_CONTROLLER_AXIS_LEFTY);
	mState.Controller.mLeftStick = Filter2D(x, y);

	x = SDL_GameControllerGetAxis(mController,
		SDL_CONTROLLER_AXIS_RIGHTX);
	y = -SDL_GameControllerGetAxis(mController,
		SDL_CONTROLLER_AXIS_RIGHTY);
	mState.Controller.mRightStick = Filter2D(x, y);
}

void InputSystem::ProcessEvent(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEWHEEL:
		mState.Mouse.mScrollWheel = Vector2(
			static_cast<float>(event.wheel.x),
			static_cast<float>(event.wheel.y));
		break;
	default:
		break;
	}
}

void InputSystem::SetRelativeMouseMode(bool value)
{
	SDL_bool set = value ? SDL_TRUE : SDL_FALSE;
	SDL_SetRelativeMouseMode(set);

	mState.Mouse.mIsRelative = value;
}

float InputSystem::Filter1D(int input)
{
	// �f�b�h�]�[�������̒l�� 0% �Ɖ��߂���
	const int deadZone = 250;

	// �ő�l�𒴂���l�� 100% �Ɖ��߂���
	const int maxValue = 30000;

	float retVal = 0.0f;

	// ���͂̐�Βl�����
	int absValue = input > 0 ? input : -input;

	// �f�b�h�]�[�������̒l�͖�������
	if (absValue > deadZone)
	{
		// �f�b�h�]�[���ƍő�l�̊Ԃɂ���l�̏����l���v�Z
		retVal = static_cast<float>(absValue - deadZone) / (maxValue - deadZone);

		// ���������̒l�ɍ��킹��
		retVal = input > 0 ? retVal : -1.0f * retVal;

		// �l�� -1.0 ���� 1.0f �܂ł͈̔͂Ɏ��߂�
		retVal = Math::Clamp(retVal, -1.0f, 1.0f);
	}

	return retVal;
}

Vector2 InputSystem::Filter2D(int inputX, int inputY)
{
	const float deadZone = 8000.0f;
	const float maxValue = 30000.0f;

	// 2�����̃x�N�g�������
	Vector2 dir;
	dir.x = static_cast<float>(inputX);
	dir.y = static_cast<float>(inputY);

	float length = dir.Length();

	// ���� Length < deadZone �Ȃ�A���͂Ȃ��Ƃ݂Ȃ�
	if (length < deadZone)
	{
		dir = Vector2::Zero;
	}
	else
	{
		// �f�b�h�]�[���ƍő�l�̓��S�~�Ԃ̏������v�Z����
		float f = (length - deadZone) / (maxValue - deadZone);

		// f �� 0.0f ���� 1.0f�܂ł̒l�Ɏ��߂�
		f = Math::Clamp(f, 0.0f, 1.0f);

		// �x�N�g���𐳋K�����Ă��珬���l�ɃX�P�[�����O����
		dir *= f / length;
	}

	return dir;
}
