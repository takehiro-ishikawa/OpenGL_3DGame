#pragma once
#include "BaseScene.h"

// プロトタイプ宣言
class Game;

class GameScene : public BaseScene
{
public:
	GameScene(Game* game);
	~GameScene();

	void LoadSceneData() override;
	void HandleKeyPress(const struct InputState& state) override;

private:

};