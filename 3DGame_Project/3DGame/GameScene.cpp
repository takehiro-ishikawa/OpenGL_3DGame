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

	// �����̐���
	new Room(mGame, Vector3::Zero, Vector3(5000.0f, 5000.0f, 1500.0f));

	// �G�̐���
	a = new Enemy(mGame);
	a->SetPosition(Vector3(1500, -1500, 0));
	a = new Enemy(mGame);
	a->SetPosition(Vector3(1500, 1500, 0));
	a = new Enemy(mGame);
	a->SetPosition(Vector3(-1500, -1500, 0));
	a = new Enemy(mGame);
	a->SetPosition(Vector3(-1500, 1500, 0));

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
	// �|�[�Y
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