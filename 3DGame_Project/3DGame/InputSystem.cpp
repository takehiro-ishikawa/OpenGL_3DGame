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
			// キーは現在も前のフレームでも押されていない
			return ButtonState::ENone;
		}
		else
		{
			// キーは現在のフレームで押した
			return ButtonState::EPressed;
		}
	}
	else // mPrevState[keyCode]は１に違いない
	{
		if (mCurrState[keyCode] == 0)
		{
			// キーは現在のフレームで放した
			return ButtonState::EReleased;
		}
		else
		{
			// キーは押されたまま
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

	// actionNameが連想配列内に無いならEnoneを返す
	if (iter == mInputMaps.end()) return ButtonState::ENone;
	
	InputCode* code = iter->second;

	// それぞれのデバイスからの入力状態を受け取る
	ButtonState keyState = Keyboard.GetKeyState(code->keyBoard);                     // キーボード	
	ButtonState mouseState = Mouse.GetButtonState(code->mouseButton);                // マウス
	ButtonState controllerState = Controller.GetButtonState(code->controllerButton); // コントローラー

	// ENoneでないものを優先して返す
	ButtonState returnState = (keyState != ButtonState::ENone) ? keyState : ButtonState::ENone;
	if (mouseState != ButtonState::ENone) returnState = mouseState;
	if (controllerState != ButtonState::ENone) returnState = controllerState;

	return returnState;
}

Vector2 InputState::GetMappedAxis(const std::string& axisName, InputDevice& device) const
{
	auto iter = mAxisMaps.find(axisName);
	device = InputDevice::ENone;

	// actionNameが連想配列内に無いなら0を返す
	if (iter == mAxisMaps.end()) return Vector2::Zero;

	AxisCode* code = iter->second;
	Vector2 returnAxis = Vector2::Zero;

	// それぞれのデバイスからの入力状態を受け取る
	// マウス
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
	
	// キーボード
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

	// コントローラー
	// 右スティック
	if (code->isCtrlRightAxis)
	{
		inputAxis = Controller.GetRightStick();
		inputAxis.y *= -1;
	}
	// 左スティック
	else
	{
		inputAxis = Controller.GetLeftStick();
	}
	if (inputAxis != Vector2::Zero)
	{
		device = InputDevice::EController;
		returnAxis = inputAxis;
	}

	// 入力したデバイスがキーボードかコントローラなら値を正規化する
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
	// キーボード
	// 現在の入力状態を示すポインタを保存
	mState.Keyboard.mCurrState = SDL_GetKeyboardState(NULL);

	// 前回の状態を示すメモリをクリア
	memset(mState.Keyboard.mPrevState, 0, SDL_NUM_SCANCODES);

	// マウス（すべてを0に設定するだけ）
	mState.Mouse.mCurrButtons = 0;
	mState.Mouse.mPrevButtons = 0;
	mState.Mouse.mIsRelative = false;

	// コントローラ
	// 接続されているコントローラーが存在する場合は、それを取得する
	mController = SDL_GameControllerOpen(0);

	// ControllerStateの初期化
	mState.Controller.mIsConnected = (mController != nullptr);
	memset(mState.Controller.mCurrButtons, 0,
		SDL_CONTROLLER_BUTTON_MAX);
	memset(mState.Controller.mPrevButtons, 0,
		SDL_CONTROLLER_BUTTON_MAX);

	// 入力アクション名とそれに対応するデバイス毎のボタン定数の定義
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
	// 現在の状態を前の状態にコピーする
	// キーボード
	memcpy(mState.Keyboard.mPrevState,
		mState.Keyboard.mCurrState,
		SDL_NUM_SCANCODES);

	// マウス
	mState.Mouse.mPrevButtons = mState.Mouse.mCurrButtons;
	mState.Mouse.mScrollWheel = Vector2::Zero;

	// コントローラ
	memcpy(mState.Controller.mPrevButtons,
		mState.Controller.mCurrButtons,
		SDL_CONTROLLER_BUTTON_MAX);
}

void InputSystem::Update()
{
	// マウスの入力を得る
	int x = 0, y = 0;
	
	// 相対モードON
	if (mState.Mouse.mIsRelative)
	{
		mState.Mouse.mCurrButtons = 
			SDL_GetRelativeMouseState(&x, &y);
	}
	// 相対モードOFF
	else
	{
		mState.Mouse.mCurrButtons = 
			SDL_GetMouseState(&x, &y);
	}

	// ポーズを解除した直後ならマウスの入力値を0にする(Resume時にカメラの角度が変わってしまうので)
	if (mIsResumed)
	{
		x = 0; y = 0;
		mIsResumed = false;
		SDL_Log("IsResumed");
	}

	mState.Mouse.mMousePos.x = static_cast<float>(x);
	mState.Mouse.mMousePos.y = static_cast<float>(y);

	// コントローラの入力を得る
	// ボタン
	for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
	{
		mState.Controller.mCurrButtons[i] =
			SDL_GameControllerGetButton(mController,
				SDL_GameControllerButton(i));
	}

	// トリガー
	mState.Controller.mLeftTrigger =
		Filter1D(SDL_GameControllerGetAxis(mController,
			SDL_CONTROLLER_AXIS_TRIGGERLEFT));
	mState.Controller.mRightTrigger =
		Filter1D(SDL_GameControllerGetAxis(mController,
			SDL_CONTROLLER_AXIS_TRIGGERRIGHT));

	// スティック
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
	// デッドゾーン未満の値は 0% と解釈する
	const int deadZone = 250;

	// 最大値を超える値は 100% と解釈する
	const int maxValue = 30000;

	float retVal = 0.0f;

	// 入力の絶対値を取る
	int absValue = input > 0 ? input : -input;

	// デッドゾーン未満の値は無視する
	if (absValue > deadZone)
	{
		// デッドゾーンと最大値の間にある値の小数値を計算
		retVal = static_cast<float>(absValue - deadZone) / (maxValue - deadZone);

		// 符号を元の値に合わせる
		retVal = input > 0 ? retVal : -1.0f * retVal;

		// 値を -1.0 から 1.0f までの範囲に収める
		retVal = Math::Clamp(retVal, -1.0f, 1.0f);
	}

	return retVal;
}

Vector2 InputSystem::Filter2D(int inputX, int inputY)
{
	const float deadZone = 8000.0f;
	const float maxValue = 30000.0f;

	// 2次元のベクトルを作る
	Vector2 dir;
	dir.x = static_cast<float>(inputX);
	dir.y = static_cast<float>(inputY);

	float length = dir.Length();

	// もし Length < deadZone なら、入力なしとみなす
	if (length < deadZone)
	{
		dir = Vector2::Zero;
	}
	else
	{
		// デッドゾーンと最大値の同心円間の小数を計算する
		float f = (length - deadZone) / (maxValue - deadZone);

		// f を 0.0f から 1.0fまでの値に収める
		f = Math::Clamp(f, 0.0f, 1.0f);

		// ベクトルを正規化してから小数値にスケーリングする
		dir *= f / length;
	}

	return dir;
}
