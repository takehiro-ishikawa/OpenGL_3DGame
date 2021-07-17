#pragma once
#include "BaseScene.h"

// �v���g�^�C�v�錾
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