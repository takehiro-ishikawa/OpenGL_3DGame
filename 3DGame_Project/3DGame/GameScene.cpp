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
	// �Q�[���v���C��Ԃɐݒ�
	mGame->SetState(Game::GameState::EGameplay);
	// �J�������b�N�̑��΃}�E�X���[�h��L���ɂ���
	mGame->GetInputSystem()->SetRelativeMouseMode(true);
	// �N���A���邽�߂ɑ��ΓI�Ɏ擾���邽�߂ɍŏ��̌Ăяo�����s��
	SDL_GetRelativeMouseState(nullptr, nullptr);

	// �v���C���[�̐���
	Player* player = new Player(mGame);
	mGame->SetPlayer(player);

	// �A�N�^�[���쐬����
	Actor* a;
	Quaternion q;

	for (int i = 0; i < 1; i++)
	{
		a = new Enemy(mGame);
		a->SetPosition(Vector3(1000, -1200 + (i * 300), 0));
	}

	// ���̐ݒ�
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

	// ��/�E�̕�
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
	// �O��/���
	for (int i = 0; i < 10; i++)
	{
		a = new PlaneActor(mGame);
		a->SetPosition(Vector3(start - size * 0.5f, start + i * size, 0.0f));
		a->SetRotation(q);

		a = new PlaneActor(mGame);
		a->SetPosition(Vector3(-start - size * 0.5f, start + i * size, 0.0f));
		a->SetRotation(q);
	}

	// ���s�����̐ݒ�
	mGame->GetRenderer()->SetAmbientLight(Vector3(0.4f, 0.4f, 0.4f));
	DirectionalLight& dir = mGame->GetRenderer()->GetDirectionalLight();
	dir.mDirection = Vector3(-0.17f, -0.55f, -0.55f);
	dir.mDiffuseColor = Vector3(0.7f, 0.7f, 0.7f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	// UI�v�f�̍쐬
	HUD* hud = new HUD(mGame);
	mGame->SetHUD(hud);

	// �_�������b�V�������[�h
	Mesh* pointLightMesh = mGame->GetRenderer()->GetMesh("Assets/Models/Sphere.fbx");
	mGame->GetRenderer()->SetPointLightMesh(pointLightMesh);
}

void GameScene::HandleKeyPress(const struct InputState& state)
{
	if (state.GetMappedButtonState(INPUT_MENU) == ButtonState::EPressed)
	{
		// PauseMenu���쐬����
		new PauseMenu(mGame);
	}

	if (state.Keyboard.GetKeyState(SDL_SCANCODE_I) == ButtonState::EPressed)
	{
		// �}�X�^�[�{�����[�������炷
		float volume = mGame->GetAudioSystem()->GetBusVolume("bus:/");
		volume = Math::Max(0.0f, volume - 0.1f);
		mGame->GetAudioSystem()->SetBusVolume("bus:/", volume);
	}

	if (state.Keyboard.GetKeyState(SDL_SCANCODE_O) == ButtonState::EPressed)
	{
		// �}�X�^�[�{�����[���𑝂₷
		float volume = mGame->GetAudioSystem()->GetBusVolume("bus:/");
		volume = Math::Min(1.0f, volume + 0.1f);
		mGame->GetAudioSystem()->SetBusVolume("bus:/", volume);
	}
}