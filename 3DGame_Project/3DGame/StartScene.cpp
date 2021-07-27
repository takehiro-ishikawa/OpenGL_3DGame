#include "StartScene.h"
#include "Game.h"
#include "MainMenu.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "InputSystem.h"
#include "PlaneActor.h"
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
	a = new Enemy(mGame);
	a->SetPosition(Vector3(1000, 0, 0));

	// 床の設定
	const float start = -2500.0f;
	const float size = 500.0f;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			a = new PlaneActor(mGame);
			Vector3 pos = Vector3(start + i * size, start + j * size, 0);
			a->SetPosition(pos);
		}
	}

	// 平行光源の設定
	mGame->GetRenderer()->SetAmbientLight(Vector3(0.6f, 0.6f, 0.6f));
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