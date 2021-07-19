#include "Game.h"
#include <algorithm>
#include <iostream>
#include "Renderer.h"
#include "AudioSystem.h"
#include "PhysWorld.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "PlaneActor.h"
#include "Player.h"
#include <SDL/SDL_ttf.h>
#include "UIScreen.h"
#include "PauseMenu.h"
#include "HUD.h"
#include "Font.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include "Skeleton.h"
#include "Animation.h"
#include "SkeletalMeshComponent.h"
#include "PointLightComponent.h"
#include "InputSystem.h"
#include "GameScene.h"
#include "StartScene.h"

Game::Game()
	:mRenderer(nullptr)
	, mAudioSystem(nullptr)
	, mPhysWorld(nullptr)
	, mGameState(GameState::EPaused)
	, mUpdatingActors(false)
	, mCurrentScene(nullptr)
{
	
}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// InputSystemを作成する
	mInputSystem = new InputSystem();
	if (!mInputSystem->Initialize())
	{
		SDL_Log("Failed to initialize input system");
		return false;
	}

	// レンダラーを作成する
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(1024.0f, 768.0f)) // 1024 768
	{
		SDL_Log("Failed to initialize renderer");
		delete mRenderer;
		mRenderer = nullptr;
		return false;
	}

	// オーディオを作成する
	mAudioSystem = new AudioSystem(this);
	if (!mAudioSystem->Initialize())
	{
		SDL_Log("Failed to initialize audio system");
		delete mAudioSystem;
		mAudioSystem = nullptr;
		return false;
	}

	// PhysWorldを作成する
	mPhysWorld = new PhysWorld(this);

	// SDL_ttfの初期化
	if (TTF_Init() != 0)
	{
		SDL_Log("Failed to initialize SDL_ttf");
		return false;
	}

	mTicksCount = SDL_GetTicks();

	// 最初はスタートSceneをロードする
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

		// シーンをロードする場合
		if (mCurrentScene->GetIsLoad())
		{
			// 現在のシーンで生成した全てのアクター、UIを削除
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
	//１フレーム前の入力状態を記録しておく
	mInputSystem->PrepareForUpdate();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mGameState = GameState::EQuit;
			break;
		default:
			break;
		}
	}

	mInputSystem->Update();
	const InputState& state = mInputSystem->GetState();

	// ゲームプレイ状態なら全ての入力アクションがゲームワールドに流される
	if (mGameState == GameState::EGameplay)
	{
		mCurrentScene->HandleKeyPress(state);
		HandleKeyPress(state);
		for (auto actor : mActors)
		{
			if (actor->GetState() == Actor::ActorState::EActive)
			{
				actor->ProcessInput(state);
			}
		}
	}
	// ポーズ状態では全ての入力アクションがUIスタックのトップにあるUI画面に流される
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
		// マスターボリュームを減らす
		float volume = mAudioSystem->GetBusVolume("bus:/");
		volume = Math::Max(0.0f, volume - 0.1f);
		mAudioSystem->SetBusVolume("bus:/", volume);
		std::cout << "111" << std::endl;
	}

	if (state.Keyboard.GetKeyState(SDL_SCANCODE_O) == ButtonState::EPressed)
	{
		// マスターボリュームを増やす
		float volume = mAudioSystem->GetBusVolume("bus:/");
		volume = Math::Min(1.0f, volume + 0.1f);
		mAudioSystem->SetBusVolume("bus:/", volume);
	}
}

void Game::UpdateGame()
{
	// デルタタイムを計算する
	// 最後のフレームから16msが経過するまで待つ
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	if (mGameState == GameState::EGameplay)
	{
		// 全てのアクターを更新する
		mUpdatingActors = true;
		for (auto actor : mActors)
		{
			actor->Update(deltaTime);
		}
		mUpdatingActors = false;

		// 保留中のアクターをmActorsに移動します
		for (auto pending : mPendingActors)
		{
			pending->ComputeWorldTransform();
			mActors.emplace_back(pending);
		}
		mPendingActors.clear();

		// 死んだアクターを一時的配列に追加します
		std::vector<Actor*> deadActors;
		for (auto actor : mActors)
		{
			if (actor->GetState() == Actor::ActorState::EDead)
			{
				deadActors.emplace_back(actor);
			}
		}

		// 死んだアクターを削除します（mActorsからそれらを削除します）
		for (auto actor : deadActors)
		{
			delete actor;
		}
	}

	// オーディオシステムを更新する
	mAudioSystem->Update(deltaTime);

	// 全てのUIScreenを更新する
	for (auto ui : mUIStack)
	{
		if (ui->GetState() == UIScreen::EActive)
		{
			ui->Update(deltaTime);
		}
	}

	// Enclosing状態のUIScreenを削除する
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
	// アクターを削除
	// ~ActorはRemoveActorを呼び出すため、別のスタイルのループを使用する必要がある
	while (!mActors.empty())
	{
		delete mActors.back();
	}
	mActors.clear();

	// UIスタックを削除
	while (!mUIStack.empty())
	{
		delete mUIStack.back();
		mUIStack.pop_back();
	}
	mUIStack.clear();

	// レンダラーを削除
	if (mRenderer)
	{
		mRenderer->UnloadData();
	}

	// フォントを削除
	for (auto f : mFonts)
	{
		f.second->Unload();
		delete f.second;
	}
	mFonts.clear();

	// スケルトンを削除
	for (auto s : mSkeletons)
	{
		delete s.second;
	}
	mSkeletons.clear();

	// アニメーションを削除
	for (auto a : mAnims)
	{
		delete a.second;
	}
	mAnims.clear();
}

void Game::Shutdown()
{
	UnloadData();
	TTF_Quit(); 

	// InputSystemを破棄
	mInputSystem->Shutdown();
	delete mInputSystem;

	// PhysWorldを破棄
	delete mPhysWorld;

	// Rendererを破棄
	if (mRenderer)
	{
		mRenderer->Shutdown();
	}

	// AudioSystemを破棄
	if (mAudioSystem)
	{
		mAudioSystem->Shutdown();
	}

	SDL_Quit();
}

void Game::LoadScene(BaseScene* loadScene)
{
	mLoadScene = loadScene;

	// 現在のシーンの破棄を予約
	mCurrentScene->SetIsLoad(true);
}

void Game::AddActor(Actor* actor)
{
	// アクターを更新する場合は、mPendingActorsに追加する必要がある
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
	// actorは保留中のアクター配列にあるか？
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// 対象のアクターを配列の最後に移動して削除する（コピーを直接消去しないこと）
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// actorはアクター配列の中にあるか？
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// 対象のアクターを配列の最後に移動して削除する（コピーを直接消去しないこと）
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

Font* Game::GetFont(const std::string& fileName)
{
	auto iter = mFonts.find(fileName);

	// fileNameのフォントがmFontsにあった場合
	if (iter != mFonts.end())
	{
		// そのフォントを返す
		return iter->second;
	}
	// 無かった場合
	else
	{
		// フォントをファイルからロードする
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

Skeleton* Game::GetSkeleton(const std::string& fileName)
{
	auto iter = mSkeletons.find(fileName);
	if (iter != mSkeletons.end())
	{
		return iter->second;
	}
	else
	{
		Skeleton* sk = new Skeleton();
		if (sk->Load(fileName))
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
	auto iter = mAnims.find(animationName);
	if (iter != mAnims.end())
	{
		return iter->second;
	}
	else
	{
		Animation* anim = new Animation();
		if (anim->Load(fileName, animationName))
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

void Game::AddPlane(PlaneActor* plane)
{
	mPlanes.emplace_back(plane);
}

void Game::RemovePlane(PlaneActor* plane)
{
	auto iter = std::find(mPlanes.begin(), mPlanes.end(), plane);
	mPlanes.erase(iter);
}