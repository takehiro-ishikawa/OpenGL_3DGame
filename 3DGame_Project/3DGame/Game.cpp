#include "Game.h"
#include <algorithm>
#include <iostream>
#include "Renderer.h"
#include "Audio.h"
#include "PhysWorld.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "Mesh.h"
#include "Field.h"
#include "Player.h"
#include <SDL/SDL_ttf.h>
#include "UIScreen.h"
#include "Menu.h"
#include "HUD.h"
#include "Font.h"
#include <fstream>
#include <sstream>
#include "SkeletalAnimation.h"
#include "PointLightComponent.h"
#include "InputSystem.h"
#include "Scene.h"
#include "FBXData.h"
#include <iomanip>

Game::Game()
	:mRenderer(nullptr)
	, mAudioSystem(nullptr)
	, mPhysWorld(nullptr)
	, mGameState(GameState::EPaused)
	, mUpdatingActors(false)
	, mCurrentScene(nullptr)
	, mWaitTime(16.0f)
{
	
}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// InputSystem���쐬����
	mInputSystem = new InputSystem();
	if (!mInputSystem->Initialize())
	{
		SDL_Log("Failed to initialize input system");
		return false;
	}

	// �����_���[���쐬����
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		SDL_Log("Failed to initialize renderer");
		delete mRenderer;
		mRenderer = nullptr;
		return false;
	}

	// �I�[�f�B�I���쐬����
	mAudioSystem = new AudioSystem(this);
	if (!mAudioSystem->Initialize())
	{
		SDL_Log("Failed to initialize audio system");
		delete mAudioSystem;
		mAudioSystem = nullptr;
		return false;
	}

	// PhysWorld���쐬����
	mPhysWorld = new PhysWorld(this);

	// SDL_ttf�̏�����
	if (TTF_Init() != 0)
	{
		SDL_Log("Failed to initialize SDL_ttf");
		return false;
	}

	// �ݒ肵���t���[�����[�g����mWaitTime���v�Z����
	mWaitTime = static_cast<int>(1 / FRAME_RATE * 1000);

	mTicksCount = SDL_GetTicks();

	// �ŏ��̓X�^�[�gScene�����[�h����
	mCurrentScene = new StartScene(this);
	mCurrentScene->LoadSceneData();

	return true;
}

void Game::RunLoop()
{
	while (mGameState != GameState::EQuit)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();

		// �V�[�������[�h����ꍇ
		if (mCurrentScene->GetIsLoad())
		{
			// ���݂̃V�[���Ő��������S�ẴA�N�^�[�AUI���폜
			UnloadData();

			delete mCurrentScene;
			mCurrentScene = mLoadScene;
			mLoadScene = nullptr;

			mCurrentScene->LoadSceneData();

			mCurrentScene->SetIsLoad(false);
		}
	}
}

void Game::ProcessInput()
{
	//�P�t���[���O�̓��͏�Ԃ��L�^���Ă���
	mInputSystem->PrepareForUpdate();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		default:
			break;
		}
	}

	// �f�o�C�X����̓��͏����擾
	mInputSystem->Update();
	const InputState& state = mInputSystem->GetState();

	// �Q�[���v���C��ԂȂ�S�Ă̓��̓A�N�V�������Q�[�����[���h�ƁAUI�X�^�b�N�̃g�b�v�ɂ��鋖���ꂽUI��ʂɗ������
	if (mGameState == GameState::EGameplay)
	{
		mCurrentScene->HandleKeyPress(state);
		HandleKeyPress(state);
		// �A�N�^�[�ւ̓���
		for (auto actor : mActors)
		{
			if (actor->GetState() == Actor::ActorState::EActive)
			{
				actor->ProcessInput(state);
			}
		}

		// �����ꂽUI�ւ̓���
		if (!mUIStack.empty() && mUIStack.back()->GetIsInputAccept())
		{
			mUIStack.back()->HandleKeyPress(state);
			mUIStack.back()->ProcessInput(state);
		}
	}
	// �|�[�Y��Ԃł͑S�Ă̓��̓A�N�V������UI�X�^�b�N�̃g�b�v�ɂ���UI��ʂɗ������
	else if (!mUIStack.empty())
	{
		mUIStack.back()->HandleKeyPress(state);
		mUIStack.back()->ProcessInput(state);
	}
}

void Game::HandleKeyPress(const struct InputState& state)
{
	if (state.Keyboard.GetKeyState(SDL_SCANCODE_I) == ButtonState::EPressed)
	{
		// �}�X�^�[�{�����[�������炷
		float volume = mAudioSystem->GetBusVolume("bus:/");
		volume = Math::Max(0.0f, volume - 0.1f);
		mAudioSystem->SetBusVolume("bus:/", volume);
		std::cout << "111" << std::endl;
	}

	if (state.Keyboard.GetKeyState(SDL_SCANCODE_O) == ButtonState::EPressed)
	{
		// �}�X�^�[�{�����[���𑝂₷
		float volume = mAudioSystem->GetBusVolume("bus:/");
		volume = Math::Min(1.0f, volume + 0.1f);
		mAudioSystem->SetBusVolume("bus:/", volume);
	}
}

void Game::UpdateGame()
{
	// �t���[������(�Ō�̃t���[������mWaitTime�~���b���o�߂���܂ő҂�)
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + mWaitTime));

	// �f���^�^�C�����v�Z
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// �t���[�����[�g���v��
	mFrameRate = 1 / deltaTime;

	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	if (mGameState == GameState::EGameplay)
	{
		// �S�ẴA�N�^�[���X�V����
		mUpdatingActors = true;
		for (auto actor : mActors)
		{
			actor->Update(deltaTime);
		}
		mUpdatingActors = false;

		// �ۗ����̃A�N�^�[��mActors�Ɉړ����܂�
		for (auto pending : mPendingActors)
		{
			pending->ComputeWorldTransform();
			mActors.emplace_back(pending);
		}
		mPendingActors.clear();

		// ���񂾃A�N�^�[���ꎞ�I�z��ɒǉ����܂�
		std::vector<Actor*> deadActors;
		for (auto actor : mActors)
		{
			if (actor->GetState() == Actor::ActorState::EDead)
			{
				deadActors.emplace_back(actor);
			}
		}

		// ���񂾃A�N�^�[���폜���܂��imActors���炻�����폜���܂��j
		for (auto actor : deadActors)
		{
			delete actor;
		}
	}

	// �I�[�f�B�I�V�X�e�����X�V����
	mAudioSystem->Update(deltaTime);

	// �S�Ă�UIScreen���X�V����
	for (auto ui : mUIStack)
	{
		if (ui->GetState() == UIScreen::EActive)
		{
			ui->Update(deltaTime);
		}
	}

	// Enclosing��Ԃ�UIScreen���폜����
	auto iter = mUIStack.begin();
	while (iter != mUIStack.end())
	{
		if ((*iter)->GetState() == UIScreen::EClosing)
		{
			delete *iter;
			iter = mUIStack.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void Game::GenerateOutput()
{
	mRenderer->Draw();
}

void Game::UnloadData()
{
	// �A�N�^�[���폜
	// ~Actor��RemoveActor���Ăяo�����߁A�ʂ̃X�^�C���̃��[�v���g�p����K�v������
	while (!mActors.empty())
	{
		delete mActors.back();
	}
	mActors.clear();

	// UI�X�^�b�N���폜
	while (!mUIStack.empty())
	{
		delete mUIStack.back();
		mUIStack.pop_back();
	}
	mUIStack.clear();

	// �e�N�X�`���A���b�V�����폜
	if (mRenderer)
	{
		mRenderer->UnloadData();
	}

	// �t�H���g���폜
	for (auto f : mFonts)
	{
		f.second->Unload();
		delete f.second;
	}
	mFonts.clear();

	// FBXData���폜
	for (auto f : mFBXData)
	{
		FBXData* d = f.second;
		delete d;
	}
	mFBXData.clear();

	// �X�P���g�����폜
	for (auto s : mSkeletons)
	{
		Skeleton* d = s.second;
		delete d;
	}
	mSkeletons.clear();

	// �A�j���[�V�������폜
	for (auto a : mAnims)
	{
		Animation* d = a.second;
		delete d;
	}
	mAnims.clear();
}

void Game::Shutdown()
{
	UnloadData();
	TTF_Quit(); 

	// InputSystem��j��
	mInputSystem->Shutdown();
	delete mInputSystem;

	// PhysWorld��j��
	delete mPhysWorld;

	// Renderer��j��
	if (mRenderer)
	{
		mRenderer->Shutdown();
	}

	// AudioSystem��j��
	if (mAudioSystem)
	{
		mAudioSystem->Shutdown();
	}

	SDL_Quit();
}

void Game::LoadScene(BaseScene* loadScene)
{
	mLoadScene = loadScene;

	// ���݂̃V�[���̔j����\��
	mCurrentScene->SetIsLoad(true);
}

void Game::AddActor(Actor* actor)
{
	// �A�N�^�[���X�V����ꍇ�́AmPendingActors�ɒǉ�����K�v������
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// actor�͕ۗ����̃A�N�^�[�z��ɂ��邩�H
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// �Ώۂ̃A�N�^�[��z��̍Ō�Ɉړ����č폜����i�R�s�[�𒼐ڏ������Ȃ����Ɓj
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// actor�̓A�N�^�[�z��̒��ɂ��邩�H
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// �Ώۂ̃A�N�^�[��z��̍Ō�Ɉړ����č폜����i�R�s�[�𒼐ڏ������Ȃ����Ɓj
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

Font* Game::GetFont(const std::string& fileName)
{
	// ���[�h�ς݂��m�F
	auto iter = mFonts.find(fileName);
	// ���[�h�ς�
	if (iter != mFonts.end())
	{
		// ���̃t�H���g��Ԃ�
		return iter->second;
	}
	// �����[�h
	else
	{
		// �t�H���g���t�@�C�����烍�[�h����
		Font* font = new Font(this);
		if (font->Load(fileName))
		{
			mFonts.emplace(fileName, font);
		}
		else
		{
			font->Unload();
			delete font;
			font = nullptr;
		}
		return font;
	}
}

void Game::PushUI(UIScreen* screen)
{
	mUIStack.emplace_back(screen);
}

FBXData* Game::GetFBXData(const std::string& fileName)
{
	// ���[�h�ς݂��m�F
	auto iter = mFBXData.find(fileName);
	// ���[�h�ς�
	if (iter != mFBXData.end())
	{
		return iter->second;
	}
	// �����[�h
	{
		std::cout << fileName << std::endl;
		FBXData* data = new FBXData(fileName.c_str());
		mFBXData.emplace(fileName, data);

		return data;
	}
}

Skeleton* Game::GetSkeleton(const std::string& fileName)
{
	// ���[�h�ς݂��m�F
	auto iter = mSkeletons.find(fileName);
	// ���[�h�ς�
	if (iter != mSkeletons.end())
	{
		return iter->second;
	}
	// �����[�h
	else
	{
		Skeleton* sk = new Skeleton();
		if (sk->Load(GetFBXData(fileName)))
		{
			mSkeletons.emplace(fileName, sk);
		}
		else
		{
			delete sk;
			sk = nullptr;
		}
		return sk;
	}
}

Animation* Game::GetAnimation(const std::string& animationName, const std::string& fileName)
{
	// ���[�h�ς݂��m�F
	auto iter = mAnims.find(animationName);
	// ���[�h�ς�
	if (iter != mAnims.end())
	{
		return iter->second;
	}
	// �����[�h
	else
	{
		Animation* anim = new Animation();
		if (anim->Load(GetFBXData(fileName), animationName))
		{
			mAnims.emplace(animationName, anim);
		}
		else
		{
			delete anim;
			anim = nullptr;
		}
		return anim;
	}
}