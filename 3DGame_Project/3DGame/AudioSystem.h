#pragma once
#include <unordered_map>
#include <string>
#include "SoundEvent.h"
#include "Math.h"

// �T�E���h�̌Ăяo����
#define SE_OK             "event:/Button_OK"     // OK�{�^��
#define SE_CANCEL         "event:/Button_Cancel" // �L�����Z���{�^��
#define SE_SELECT         "event:/ButtonSelect"  // �{�^���I��
#define SE_SHOOT_P        "event:/PlayerShoot"   // �v���C���[�̒e���ˉ�
#define SE_SHOOT_E        "event:/EnemyShoot"    // �G�̒e���ˉ�
#define SE_JUMP           "event:/Jump"          // �W�����v
#define SE_DASH           "event:/DashStart"     // �_�b�V������
#define SE_FOOTSTEP_WALK  "event:/FootStep_Walk" // ���s���̑���
#define SE_FOOTSTEP_DASH  "event:/FootStep_Run"  // �_�b�V�����̑���
#define SE_SHOOT_STANDBY  "event:/ShootStandBy"  // �ˌ��̐�
#define SE_SLASH          "event:/Slash"         // �ߐڍU��
#define SE_FOOTSTEP_ENEMY "event:/EnemyFootStep" // �G�̑���
#define SE_ENEMY_DOWN     "event:/EnemyDown"     // �G�̓|��鉹
#define SE_ENEMY_DISCOVER "event:/EnemyDiscover" // �G�̃v���C���[������

#pragma region �v���g�^�C�v�錾
class Game;
class SoundEvent;
#pragma endregion

namespace FMOD
{
	class System;
	namespace Studio
	{
		class Bank;             // �C�x���g�A�T���v���f�[�^�A�X�g���[�~���O�f�[�^������R���e�i
		class EventDescription; // �C�x���g�̌^�̂悤�Ȃ���
		class EventInstance;    // �^�̃C���X�^���X
		class System;
		class Bus;
	};
};

// �S�ẴT�E���h���Ǘ�
class AudioSystem
{
public:
	AudioSystem(Game* game);
	~AudioSystem();

	bool Initialize();
	void Shutdown();
	void Update(float deltaTime);

	void LoadBank(const std::string& name);
	void UnloadBank(const std::string& name);
	void UnloadAllBanks();

	SoundEvent PlayEvent(const std::string& name);

	// �r���[�s�񂩂烊�X�i�[��ݒ肷��
	void SetListener(const Matrix4& viewMatrix);

	// �o�X�̐���
	float GetBusVolume(const std::string& name) const;
	bool GetBusPaused(const std::string& name) const;
	void SetBusVolume(const std::string& name, float volume);
	void SetBusPaused(const std::string& name, bool pause);

protected:
	// SoundEvent���������̊֐��ɃA�N�Z�X�ł���悤�ɂ���B
	friend SoundEvent;
	FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);

private:
	// �C�x���g�C���X�^���X�Ɏg�p���鎟��ID��ǐՂ���
	static unsigned int sNextID;

	Game* mGame;

	// FMOD studio�V�X�e��
	FMOD::Studio::System* mSystem;

	// FMOD Low-level�V�X�e���i�K�v���̂��߂Ɂj
	FMOD::System* mLowLevelSystem;

	// ���[�h���ꂽ�o���N�̘A�z�z��
	std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks;

	// �o�X�̘A�z�z��
	std::unordered_map<std::string, FMOD::Studio::Bus*> mBuses;

	// �C�x���g������ EventDescription �ւ̘A�z�z��
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;

	// �C�x���g�C���X�^���X�ւ̘A�z�z��
	std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstances;
};