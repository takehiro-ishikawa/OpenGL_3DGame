#pragma once
#include "BaseScene.h"

// プロトタイプ宣言
class Game;
class CameraActor;

class StartScene : public BaseScene
{
public:
	StartScene(Game* game);
	~StartScene();

	void LoadSceneData() override;
	void HandleKeyPress(const struct InputState& state) override;

private:
	CameraActor* mPointCamera;
};