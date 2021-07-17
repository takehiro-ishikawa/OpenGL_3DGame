#pragma once

// プロトタイプ宣言
class Game;

class BaseScene
{
public:
	BaseScene(class Game* game);
	~BaseScene();

	virtual void LoadSceneData() = 0;
	virtual void HandleKeyPress(const struct InputState& state) = 0;

	bool GetIsLoad() { return mIsLoad; }
	void SetIsLoad(bool value) { mIsLoad = value; }

protected:
	Game* mGame;

	// 別のSceneをロードするか？
	bool mIsLoad;
};