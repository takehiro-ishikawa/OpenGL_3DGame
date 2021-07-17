#pragma once

// �v���g�^�C�v�錾
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

	// �ʂ�Scene�����[�h���邩�H
	bool mIsLoad;
};