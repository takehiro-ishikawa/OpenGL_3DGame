#pragma once

// プロトタイプ宣言
class Game;
class CameraActor;

// ゲームシーンの基底クラス
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

// スタートシーン
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

// ゲームシーン
class GameScene : public BaseScene
{
public:
	GameScene(Game* game);
	~GameScene();

	void LoadSceneData() override;
	void HandleKeyPress(const struct InputState& state) override;

private:

};