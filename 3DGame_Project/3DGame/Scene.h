#pragma once

// �v���g�^�C�v�錾
class Game;
class CameraActor;

// �Q�[���V�[���̊��N���X
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

	// �ʂ�Scene�����[�h���邩�H
	bool mIsLoad;
};

// �X�^�[�g�V�[��
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

// �Q�[���V�[��
class GameScene : public BaseScene
{
public:
	GameScene(Game* game);
	~GameScene();

	void LoadSceneData() override;
	void HandleKeyPress(const struct InputState& state) override;

private:

};