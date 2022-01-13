#include "Scene.h"
#include "Game.h"
#include "Audio.h"
#include "Actor.h"
#include "Player.h"
#include "Mesh.h"
#include "Field.h"
#include "Renderer.h"
#include "SpriteComponent.h"
#include "HUD.h"
#include "InputSystem.h"
#include "Menu.h"
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

	// 部屋の生成
	new Room(mGame, Vector3::Zero, Vector3(5000.0f, 5000.0f, 1500.0f));

	// 敵の生成
	a = new Enemy(mGame);
	a->SetPosition(Vector3(1500, -1500, 0));
	a = new Enemy(mGame);
	a->SetPosition(Vector3(1500, 1500, 0));
	a = new Enemy(mGame);
	a->SetPosition(Vector3(-1500, -1500, 0));
	a = new Enemy(mGame);
	a->SetPosition(Vector3(-1500, 1500, 0));

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
	// ポーズ
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