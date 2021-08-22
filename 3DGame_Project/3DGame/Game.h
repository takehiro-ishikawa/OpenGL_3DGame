#pragma once
#include <SDL/SDL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"

#define SCREEN_WIDTH 1024.0f  // ��ʂ̉���
#define SCREEN_HEIGHT 768.0f  // ��ʂ̏c��
#define FRAME_RATE     60.0f  // �t���[�����[�g(FPS)
#define FIELD_OF_VIEW  70.0f  // ��p
#define NEAR_PLANE     10.0f  // �ߐڕ���
#define FAR_PLANE   10000.0f  // ��������

#pragma region �v���g�^�C�v�錾
class Actor;
class UIScreen;
class PlaneActor;
class Renderer;
class AudioSystem;
class PhysWorld;
class HUD;
class InputSystem;
class Font;
class Skeleton;
class Animation;
class Player;
class BaseScene;
class FBXData;
#pragma endregion

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void LoadScene(BaseScene* loadScene);

	void AddActor(Actor* actor);
	void RemoveActor(Actor* actor);

	void PushUI(UIScreen* screen); // �w���UIScreen���X�^�b�N�Ƀv�b�V������

	// �Q�[���̏��
	enum class GameState
	{
		EGameplay, // �Q�[���v���C
		EPaused,   // �|�[�Y
		EQuit      // �I��
	};

	// �Q�b�^�[/�Z�b�^�[
	Renderer* GetRenderer() { return mRenderer; }
	AudioSystem* GetAudioSystem() { return mAudioSystem; }
	PhysWorld* GetPhysWorld() { return mPhysWorld; }
	HUD* GetHUD() { return mHUD; }
	void SetHUD(HUD* hud) { mHUD = hud; }
	InputSystem* GetInputSystem() { return mInputSystem; }

	GameState GetState() const { return mGameState; }
	void SetState(GameState state) { mGameState = state; }

	const std::vector<UIScreen*>& GetUIStack() { return mUIStack; }// UI�X�^�b�N�S�̂��Q�ƂŕԂ�
	Font* GetFont(const std::string& fileName);                    // �w�肵���t�H���g���擾����

	FBXData* GetFBXData(const std::string& fileName);
	Skeleton* GetSkeleton(const std::string& fileName);
	Animation* GetAnimation(const std::string& animationName, const std::string& fileName);

	Player* GetPlayer() { return mPlayer; }
	void SetPlayer(Player* tpsActor) { mPlayer = tpsActor; }
	float GetFrameRate() { return mFrameRate; }

private:
	void ProcessInput();
	void HandleKeyPress(const struct InputState& state);
	void UpdateGame();
	void GenerateOutput();
	void UnloadData();

	// �Q�[�����\������e�v�f�̐���n
	Renderer* mRenderer;       // �`��
	AudioSystem* mAudioSystem; // �T�E���h
	PhysWorld* mPhysWorld;     // �����蔻��
	HUD* mHUD;                 // HUD
	InputSystem* mInputSystem; // ����

	// �V�[��
	BaseScene* mCurrentScene;  // ���݂̃Q�[���V�[��
	BaseScene* mLoadScene;     // ���Ƀ��[�h����Q�[���V�[��

	// �A�N�^�[
	std::vector<Actor*> mActors;        // �Q�[�����̂��ׂẴA�N�^�[
	std::vector<Actor*> mPendingActors; // �ۗ����̃A�N�^�[

	// UI
	std::vector<UIScreen*> mUIStack;               // �Q�[���p��UI�X�^�b�N
	std::unordered_map<std::string, Font*> mFonts; // �t�H���g�̃}�b�v
	
	// FBX�t�@�C������擾����f�[�^
	std::unordered_map<std::string, FBXData*> mFBXData;    // ���[�h���ꂽFBXData�̃}�b�v
	std::unordered_map<std::string, Skeleton*> mSkeletons; // ���[�h���ꂽ�X�P���g���̃}�b�v
	std::unordered_map<std::string, Animation*> mAnims;    // ���[�h���ꂽ�A�j���[�V�����̃}�b�v

	Uint32 mTicksCount;   // �o�ߎ��Ԃ�ێ�(�~���b)
	GameState mGameState; // ���݂̃Q�[���̏��
	bool mUpdatingActors; // ���݃A�N�^�[���X�V���Ă��邩
	int mWaitTime;        // �Q�[���̍X�V��҂���(�~���b)
	float mFrameRate;     // ���݂̃t���[�����[�g

	// ���삷��v���C���[
	Player* mPlayer; 
};