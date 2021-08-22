#pragma once
#include <SDL/SDL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"

#define SCREEN_WIDTH 1024.0f  // 画面の横幅
#define SCREEN_HEIGHT 768.0f  // 画面の縦幅
#define FRAME_RATE     60.0f  // フレームレート(FPS)
#define FIELD_OF_VIEW  70.0f  // 画角
#define NEAR_PLANE     10.0f  // 近接平面
#define FAR_PLANE   10000.0f  // 遠方平面

#pragma region プロトタイプ宣言
class Actor;
class UIScreen;
class PlaneActor;
class Renderer;
class AudioSystem;
class PhysWorld;
class HUD;
class InputSystem;
class Font;
class Skeleton;
class Animation;
class Player;
class BaseScene;
class FBXData;
#pragma endregion

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void LoadScene(BaseScene* loadScene);

	void AddActor(Actor* actor);
	void RemoveActor(Actor* actor);

	void PushUI(UIScreen* screen); // 指定のUIScreenをスタックにプッシュする

	// ゲームの状態
	enum class GameState
	{
		EGameplay, // ゲームプレイ
		EPaused,   // ポーズ
		EQuit      // 終了
	};

	// ゲッター/セッター
	Renderer* GetRenderer() { return mRenderer; }
	AudioSystem* GetAudioSystem() { return mAudioSystem; }
	PhysWorld* GetPhysWorld() { return mPhysWorld; }
	HUD* GetHUD() { return mHUD; }
	void SetHUD(HUD* hud) { mHUD = hud; }
	InputSystem* GetInputSystem() { return mInputSystem; }

	GameState GetState() const { return mGameState; }
	void SetState(GameState state) { mGameState = state; }

	const std::vector<UIScreen*>& GetUIStack() { return mUIStack; }// UIスタック全体を参照で返す
	Font* GetFont(const std::string& fileName);                    // 指定したフォントを取得する

	FBXData* GetFBXData(const std::string& fileName);
	Skeleton* GetSkeleton(const std::string& fileName);
	Animation* GetAnimation(const std::string& animationName, const std::string& fileName);

	Player* GetPlayer() { return mPlayer; }
	void SetPlayer(Player* tpsActor) { mPlayer = tpsActor; }
	float GetFrameRate() { return mFrameRate; }

private:
	void ProcessInput();
	void HandleKeyPress(const struct InputState& state);
	void UpdateGame();
	void GenerateOutput();
	void UnloadData();

	// ゲームを構成する各要素の制御系
	Renderer* mRenderer;       // 描画
	AudioSystem* mAudioSystem; // サウンド
	PhysWorld* mPhysWorld;     // 当たり判定
	HUD* mHUD;                 // HUD
	InputSystem* mInputSystem; // 入力

	// シーン
	BaseScene* mCurrentScene;  // 現在のゲームシーン
	BaseScene* mLoadScene;     // 次にロードするゲームシーン

	// アクター
	std::vector<Actor*> mActors;        // ゲーム内のすべてのアクター
	std::vector<Actor*> mPendingActors; // 保留中のアクター

	// UI
	std::vector<UIScreen*> mUIStack;               // ゲーム用のUIスタック
	std::unordered_map<std::string, Font*> mFonts; // フォントのマップ
	
	// FBXファイルから取得するデータ
	std::unordered_map<std::string, FBXData*> mFBXData;    // ロードされたFBXDataのマップ
	std::unordered_map<std::string, Skeleton*> mSkeletons; // ロードされたスケルトンのマップ
	std::unordered_map<std::string, Animation*> mAnims;    // ロードされたアニメーションのマップ

	Uint32 mTicksCount;   // 経過時間を保持(ミリ秒)
	GameState mGameState; // 現在のゲームの状態
	bool mUpdatingActors; // 現在アクターを更新しているか
	int mWaitTime;        // ゲームの更新を待つ時間(ミリ秒)
	float mFrameRate;     // 現在のフレームレート

	// 操作するプレイヤー
	Player* mPlayer; 
};