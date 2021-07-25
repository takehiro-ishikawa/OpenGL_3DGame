#pragma once
#include <SDL/SDL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"

#define SCREEN_WIDTH  1024.0f // 画面の横幅
#define SCREEN_HEIGHT 768.0f  // 画面の縦幅

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

	void AddPlane(PlaneActor* plane);
	void RemovePlane(PlaneActor* plane);

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

	Skeleton* GetSkeleton(const std::string& fileName);
	Animation* GetAnimation(const std::string& animationName, const std::string& fileName);

	Player* GetPlayer() { return mPlayer; }
	void SetPlayer(Player* tpsActor) { mPlayer = tpsActor; }
	std::vector<PlaneActor*>& GetPlanes() { return mPlanes; }
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
	
	// アニメーション
	std::unordered_map<std::string, Skeleton*> mSkeletons; // ロードされたスケルトンのマップ
	std::unordered_map<std::string, Animation*> mAnims;    // ロードされたアニメーションのマップ

	Uint32 mTicksCount;   // 経過時間を保持(ミリ秒)
	GameState mGameState; // 現在のゲームの状態
	bool mUpdatingActors; // 現在アクターを更新しているか
	float mFrameRate;     // 現在のフレームレート

	// ゲーム固有のコード
	Player* mPlayer; // 操作するプレイヤー
	std::vector<PlaneActor*> mPlanes;
};