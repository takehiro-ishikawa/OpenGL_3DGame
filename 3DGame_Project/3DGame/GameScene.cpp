#include "GameScene.h"
#include "Game.h"
#include "AudioSystem.h"
#include "Actor.h"
#include "Player.h"
#include "MeshComponent.h"
#include "PlaneActor.h"
#include "Renderer.h"
#include "SpriteComponent.h"
#include "HUD.h"
#include "InputSystem.h"
#include "PauseMenu.h"
#include "Enemy.h"

GameScene::GameScene(Game* game)
	:BaseScene(game)
{
}

GameScene::~GameScene()
{
}

void GameScene::LoadSceneData()
{
	// ゲームプレイ状態に設定
	mGame->SetState(Game::GameState::EGameplay);
	// カメラルックの相対マウスモードを有効にする
	mGame->GetInputSystem()->SetRelativeMouseMode(true);
	// クリアするために相対的に取得するために最初の呼び出しを行う
	SDL_GetRelativeMouseState(nullptr, nullptr);

	// プレイヤーの生成
	Player* player = new Player(mGame);
	mGame->SetPlayer(player);

	// アクターを作成する
	Actor* a;
	Quaternion q;

	for (int i = 0; i < 1; i++)
	{
		a = new Enemy(mGame);
		a->SetPosition(Vector3(1000, -1200 + (i * 300), 0));
	}

	// 床の設定
	const float start = -2500.0f;
	const float size = 500.0f;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			a = new PlaneActor(mGame);
			Vector3 pos = Vector3(start + i * size, start + j * size, 0.0f);
			a->SetPosition(pos);
		}
	}

	// 左/右の壁
	q = Quaternion(Vector3::UnitX, Math::PiOver2);
	for (int i = 0; i < 10; i++)
	{
		a = new PlaneActor(mGame);
		a->SetPosition(Vector3(start + i * size, start - size * 0.5f, 0.0f));
		a->SetRotation(q);

		a = new PlaneActor(mGame);
		a->SetPosition(Vector3(start + i * size, -start - size * 0.5f, 0.0f));
		a->SetRotation(q);
	}

	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PiOver2));
	// 前壁/後壁
	for (int i = 0; i < 10; i++)
	{
		a = new PlaneActor(mGame);
		a->SetPosition(Vector3(start - size * 0.5f, start + i * size, 0.0f));
		a->SetRotation(q);

		a = new PlaneActor(mGame);
		a->SetPosition(Vector3(-start - size * 0.5f, start + i * size, 0.0f));
		a->SetRotation(q);
	}

	// 平行光源の設定
	mGame->GetRenderer()->SetAmbientLight(Vector3(0.4f, 0.4f, 0.4f));
	DirectionalLight& dir = mGame->GetRenderer()->GetDirectionalLight();
	dir.mDirection = Vector3(-0.17f, -0.55f, -0.55f);
	dir.mDiffuseColor = Vector3(0.7f, 0.7f, 0.7f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	// UI要素の作成
	HUD* hud = new HUD(mGame);
	mGame->SetHUD(hud);

	// 点光源メッシュをロード
	Mesh* pointLightMesh = mGame->GetRenderer()->GetMesh("Assets/Models/Sphere.fbx");
	mGame->GetRenderer()->SetPointLightMesh(pointLightMesh);
}

void GameScene::HandleKeyPress(const struct InputState& state)
{
	if (state.GetMappedButtonState(INPUT_MENU) == ButtonState::EPressed)
	{
		// PauseMenuを作成する
		new PauseMenu(mGame);
	}

	if (state.Keyboard.GetKeyState(SDL_SCANCODE_I) == ButtonState::EPressed)
	{
		// マスターボリュームを減らす
		float volume = mGame->GetAudioSystem()->GetBusVolume("bus:/");
		volume = Math::Max(0.0f, volume - 0.1f);
		mGame->GetAudioSystem()->SetBusVolume("bus:/", volume);
	}

	if (state.Keyboard.GetKeyState(SDL_SCANCODE_O) == ButtonState::EPressed)
	{
		// マスターボリュームを増やす
		float volume = mGame->GetAudioSystem()->GetBusVolume("bus:/");
		volume = Math::Min(1.0f, volume + 0.1f);
		mGame->GetAudioSystem()->SetBusVolume("bus:/", volume);
	}
}