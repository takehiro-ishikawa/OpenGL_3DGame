#pragma once
#include <SDL/SDL_scancode.h>
#include <SDL/SDL_gamecontroller.h>
#include <SDL/SDL_mouse.h>
#include "Math.h"
#include <string>
#include <unordered_map>

// 入力名
#define INPUT_LEFT_AXIS    "LeftAxis"
#define INPUT_RIGHT_AXIS   "RightAxis"
#define INPUT_JUMP         "Jump"
#define INPUT_UP           "Up"
#define INPUT_DOWN         "Down"
#define INPUT_SPRINT       "Sprint"
#define INPUT_FIRE         "Fire"
#define INPUT_FIRE_STANDBY "FireStandBy"

// ボタンの入力状態
enum class ButtonState
{
	ENone,     // どれでもない
	EPressed,  // 押した
	EReleased, // 放した
	EHeld      // 押したまま
};

// キーボード入力のヘルパー
class KeyboardState
{
public:
	// InputSystemはfriendクラスなので容易に更新できる
	friend class InputSystem;

	// キーの真偽値のみを取得する
	bool GetKeyValue(SDL_Scancode keyCode) const;

	// 現在と、その1つ前のフレームから状態を取得
	ButtonState GetKeyState(SDL_Scancode keyCode) const;

private:
	const Uint8* mCurrState;             // 現在の入力状態
	Uint8 mPrevState[SDL_NUM_SCANCODES]; // 1つ前の入力状態
};

// マウス入力のヘルパー
class MouseState
{
public:
	friend class InputSystem;

	// マウスポジション用
	const Vector2& GetPosition() const { return mMousePos; }
	const Vector2& GetScrollWheel() const { return mScrollWheel; }
	bool IsRelative() const { return mIsRelative; }

	// マウスボタン用
	bool GetButtonValue(int button) const;
	ButtonState GetButtonState(int button) const;

private:
	// マウスの位置を格納
	Vector2 mMousePos;

	// マウスホイールの動きを格納
	Vector2 mScrollWheel;

	// ボタンのデータを格納
	Uint32 mCurrButtons; // 現在の入力状態
	Uint32 mPrevButtons; // 1つ前の入力状態

	// 相対マウスモードか？
	bool mIsRelative;
};

// コントローラ入力のヘルパー
class ControllerState
{
public:
	friend class InputSystem;

	// ボタン用
	bool GetButtonValue(SDL_GameControllerButton button) const;
	ButtonState GetButtonState(SDL_GameControllerButton button) const;

	const Vector2& GetLeftStick() const { return mLeftStick; }
	const Vector2& GetRightStick() const { return mRightStick; }
	float GetLeftTrigger() const { return mLeftTrigger; }
	float GetRightTrigger() const { return mRightTrigger; }

	bool GetIsConnected() const { return mIsConnected; }

private:
	// 今回/前回のボタン
	Uint8 mCurrButtons[SDL_CONTROLLER_BUTTON_MAX];
	Uint8 mPrevButtons[SDL_CONTROLLER_BUTTON_MAX];

	// 左/右 アナログスティック
	Vector2 mLeftStick;
	Vector2 mRightStick;

	// 左/右 トリガー
	float mLeftTrigger;
	float mRightTrigger;

	// このコントローラは接続されているか?
	bool mIsConnected;
};

// デバイス毎の特定のボタン定数を格納するラッパー
struct InputCode
{
	SDL_Scancode keyBoard;                     // キーボード
	int mouseButton;                           // マウス
	SDL_GameControllerButton controllerButton; // コントローラー
};

// デバイス毎のAxis入力を格納するラッパー
struct AxisCode
{
	// キーボード
	SDL_Scancode vertical_High;   // 縦軸の＋方向
	SDL_Scancode vertical_Low;    // 縦軸の－方向
	SDL_Scancode horizontal_High; // 横軸の＋方向
	SDL_Scancode horizontal_Low;  // 横軸の－方向

	bool isMouseAxis;             // マウスの位置を受け取るか

	bool isCtrlRightAxis;         // コントローラーの右と左どちらのスティック入力を受け取るか
};

// 入力されたデバイス
enum class InputDevice
{
	EMouse,      // マウス
	EKeyBoard,   // キーボード
	EController, // コントローラ
	ENone        // どれでもない
};

// 現在の入力状態を格納するラッパー
struct InputState
{
	KeyboardState Keyboard;     // キーボード
	MouseState Mouse;           // マウス
	ControllerState Controller; // コントローラー

	// アクションの名前に対応するデバイスの入力状態を返す
	ButtonState GetMappedButtonState(const std::string& actionName) const;
	Vector2 GetMappedAxis(const std::string& axisName, InputDevice& device) const;

	// mInputMapsへの値の格納に使用するヘルパー関数
	void SetInputMap(const std::string& actionName,
		SDL_Scancode keyBoard, int mouseButton, SDL_GameControllerButton controllerButton);
	// mAxisMapsへの値の格納に使用するヘルパー関数
	void SetAxisMap(const std::string& axisName, SDL_Scancode verHigh, SDL_Scancode verLow,
		SDL_Scancode horHigh, SDL_Scancode horLow, bool mouseValue, bool ctrlValue);
	
	std::unordered_map<std::string, InputCode*> mInputMaps;// デバイス毎のボタン定数を格納する連想配列
	std::unordered_map<std::string, AxisCode*> mAxisMaps;  // デバイス毎のAxis入力を格納する連想配列
};

// 全ての入力処理を管理
class InputSystem
{
public:
	bool Initialize();
	void Shutdown();

	// SDL_PollEventsループの直前に呼ばれる
	void PrepareForUpdate();

	// SDL_PollEventsループの直後に呼ばれる
	void Update();

	// InputSystemでSDLイベントを処理するために呼び出される
	void ProcessEvent(union SDL_Event& event);

	const InputState& GetState() const { return mState; }

	void SetRelativeMouseMode(bool value);

	void SetIsResumed(bool value) { mIsResumed = value; }

private:
	float Filter1D(int input);                // トリガー入力の値にフィルターをかける
	Vector2 Filter2D(int inputX, int inputY); // アナログスティック入力の値にフィルターをかける

	InputState mState;
	SDL_GameController* mController;

	// ポーズを解除した直後か？
	bool mIsResumed;
};
