#pragma once
#include "UIScreen.h"
#include <vector>

#define HEALTH_POSITION Vector2(-350.0f, -300.0f)

#pragma region プロトタイプ宣言
class Game;
class Shader;
class Texture;
class TargetComponent;
#pragma endregion

// テクスチャ画像のファイルパス
#define HEALTHBAR_BACK_FILEPATH  "Assets/Textures/UI/HealthBarBack.png"  // 体力ゲージ背景
#define HEALTHBAR_FILEPATH       "Assets/Textures/UI/HealthBar.png"      // 体力ゲージ
#define HEALTHBAR_EMPTY_FILEPATH "Assets/Textures/UI/HealthBarEmpty.png" // 体力ゲージ減少部分
#define HUD_BACK                 "Assets/Textures/UI/HUD_Back.png"       // HUD背景

class HUD : public UIScreen
{
public:
	HUD(Game* game);
	~HUD();

	void Update(float deltaTime) override;
	void Draw(Shader* shader) override;

	void SetNormalizeHealth(float value) { mNormalizeHealth = value; }

	void AddTargetComponent(TargetComponent* tc);
	void RemoveTargetComponent(TargetComponent* tc);

protected:
	void UpdateCrosshair(float deltaTime);
	void UpdateRadar(float deltaTime);

	// 体力ゲージ関連
	void DrawHealthBar(Shader* shader); // 体力ゲージを描画する
	float mNormalizeHealth;             // 正規化された体力の値
	Texture* mHealthBarBack;            // 体力ゲージの背景
	Texture* mHealthBar;                // 体力ゲージ本体
	Texture* mHealthBarEmpty;           // 体力ゲージ減少部分

	Texture* mRadar;
	Texture* mBlipTex;
	Texture* mRadarArrow;

	Texture* mBack;

	// ゲーム内の全てのTargetComponent
	std::vector<TargetComponent*> mTargetComps;

	// レーダー中心から輝点への2D相対オフセット
	std::vector<Vector2> mBlips;

	// レーダーの範囲と半径
	float mRadarRange;
	float mRadarRadius;

	// 十字線が敵を捉えているか
	bool mTargetEnemy;
};
