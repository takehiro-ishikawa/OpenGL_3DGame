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

	// InputSystemを作成する
	mInputSystem = new InputSystem();
	if (!mInputSystem->Initialize())
	{
		SDL_Log("Failed to initialize input system");
		return false;
	}

	// レンダラーを作成する
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(SCREEN_WIDTH, SCREEN_HEIGHT))
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

	// 設定したフレームレートからmWaitTimeを計算する
	mWaitTime = static_cast<int>(1 / FRAME_RATE * 1000);

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
		default:
			break;
		}
	}

	// デバイスからの入力情報を取得
	mInputSystem->Update();
	const InputState& state = mInputSystem->GetState();

	// ゲームプレイ状態なら全ての入力アクションがゲームワールドと、UIスタックのトップにある許可されたUI画面に流される
	if (mGameState == GameState::EGameplay)
	{
		mCurrentScene->HandleKeyPress(state);
		HandleKeyPress(state);
		// アクターへの入力
		for (auto actor : mActors)
		{
			if (actor->GetState() == Actor::ActorState::EActive)
			{
				actor->ProcessInput(state);
			}
		}

		// 許可されたUIへの入力
		if (!mUIStack.empty() && mUIStack.back()->GetIsInputAccept())
		{
			mUIStack.back()->HandleKeyPress(state);
			mUIStack.back()->ProcessInput(state);
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
	// フレーム制限(最後のフレームからmWaitTimeミリ秒が経過するまで待つ)
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + mWaitTime));

	// デルタタイムを計算
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// フレームレートを計測
	mFrameRate = 1 / deltaTime;

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

	// テクスチャ、メッシュを削除
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

	// FBXDataを削除
	for (auto f : mFBXData)
	{
		FBXData* d = f.second;
		delete d;
	}
	mFBXData.clear();

	// スケルトンを削除
	for (auto s : mSkeletons)
	{
		Skeleton* d = s.second;
		delete d;
	}
	mSkeletons.clear();

	// アニメーションを削除
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
	// ロード済みか確認
	auto iter = mFonts.find(fileName);
	// ロード済み
	if (iter != mFonts.end())
	{
		// そのフォントを返す
		return iter->second;
	}
	// 未ロード
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

FBXData* Game::GetFBXData(const std::string& fileName)
{
	// ロード済みか確認
	auto iter = mFBXData.find(fileName);
	// ロード済み
	if (iter != mFBXData.end())
	{
		return iter->second;
	}
	// 未ロード
	{
		std::cout << fileName << std::endl;
		FBXData* data = new FBXData(fileName.c_str());
		mFBXData.emplace(fileName, data);

		return data;
	}
}

Skeleton* Game::GetSkeleton(const std::string& fileName)
{
	// ロード済みか確認
	auto iter = mSkeletons.find(fileName);
	// ロード済み
	if (iter != mSkeletons.end())
	{
		return iter->second;
	}
	// 未ロード
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
	// ロード済みか確認
	auto iter = mAnims.find(animationName);
	// ロード済み
	if (iter != mAnims.end())
	{
		return iter->second;
	}
	// 未ロード
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