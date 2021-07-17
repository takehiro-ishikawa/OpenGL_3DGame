#pragma once
#include "UIScreen.h"
#include <vector>

#pragma region プロトタイプ宣言
class Game;
class Shader;
class Texture;
class TargetComponent;
#pragma endregion

class HUD : public UIScreen
{
public:
	//（低いdraw orderは、さらにバックに対応します）
	HUD(Game* game);
	~HUD();

	void Update(float deltaTime) override;
	void Draw(Shader* shader) override;

	void AddTargetComponent(TargetComponent* tc);
	void RemoveTargetComponent(TargetComponent* tc);

protected:
	void UpdateCrosshair(float deltaTime);
	void UpdateRadar(float deltaTime);

	Texture* mHealthBar;
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
