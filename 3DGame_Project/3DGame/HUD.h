#pragma once
#include "UIScreen.h"
#include <vector>

#define HEALTH_POSITION Vector2(-350.0f, -300.0f)
#define FPS_POSITION Vector2(-470.0f, 320.0f)
#define FPS_FONT_SIZE 24

#pragma region プロトタイプ宣言
class Game;
class Shader;
class Texture;
class TargetComponent;
#pragma endregion

// テクスチャ画像のファイルパス
#define HEALTHBAR_BACK_FILEPATH  "Assets/Textures/UI/HealthBarBack.png"    // 体力ゲージ背景
#define HEALTHBAR_FILEPATH       "Assets/Textures/UI/HealthBar.png"        // 体力ゲージ
#define HEALTHBAR_EMPTY_FILEPATH "Assets/Textures/UI/HealthBarEmpty.png"   // 体力ゲージ減少部分
#define HUD_BACK                 "Assets/Textures/UI/HUD_Back.png"         // HUD背景
#define GUIDE_KEYBOARD           "Assets/Textures/UI/Guide_Keyboard.png"   // キーボード操作ガイド
#define GUIDE_CONTROLLER         "Assets/Textures/UI/Guide_Controller.png" // コントローラ操作ガイド

class HUD : public UIScreen
{
public:
	HUD(Game* game);
	~HUD();

	void Update(float deltaTime) override;
	void Draw(Shader* shader) override;

	void SetNormalizeHealth(float value) { mNormalizeHealth = value; }

protected:
	void DrawHealthBar(Shader* shader); // 体力ゲージを描画する
	void DrawFrameRate(Shader* shader); // 現在のFPSを描画する

	float mNormalizeHealth;             // 正規化された体力の値
	Texture* mHealthBarBack;            // 体力ゲージの背景
	Texture* mHealthBar;                // 体力ゲージ本体
	Texture* mHealthBarEmpty;           // 体力ゲージ減少部分

	Texture* mBack;            // 背景
	Texture* mGuideKeyBoard;   // キーボード操作ガイド
	Texture* mGuideController; // コントローラ操作ガイド
	Texture* mFrameRate;
};
