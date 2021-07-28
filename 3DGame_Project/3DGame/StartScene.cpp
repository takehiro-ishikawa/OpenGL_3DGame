#include "StartScene.h"
#include "Game.h"
#include "MainMenu.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "InputSystem.h"
#include "Room.h"
#include "CameraActor.h"
#include "Player.h"
#include "HUD.h"
#include "Enemy.h"

StartScene::StartScene(Game* game)
	:BaseScene(game)
{
}

StartScene::~StartScene()
{
}

void StartScene::LoadSceneData()
{
	mGame->SetState(Game::GameState::EGameplay);

	mPointCamera = new CameraActor(mGame);

	Actor* a;

	// 部屋の生成
	new Room(mGame, Vector3::Zero, Vector3(5000.0f, 5000.0f, 0));

	// 平行光源の設定
	mGame->GetRenderer()->SetAmbientLight(Vector3(0.4f, 0.4f, 0.4f));
	DirectionalLight& dir = mGame->GetRenderer()->GetDirectionalLight();
	dir.mDirection = Vector3(-0.17f, -0.55f, -0.55f);
	dir.mDiffuseColor = Vector3(0.7f, 0.7f, 0.7f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	// 点光源メッシュをロード
	Mesh* pointLightMesh = mGame->GetRenderer()->GetMesh("Assets/Models/Sphere.fbx");
	mGame->GetRenderer()->SetPointLightMesh(pointLightMesh);

	new MainMenu(mGame);
}

void StartScene::HandleKeyPress(const struct InputState& state)
{

}